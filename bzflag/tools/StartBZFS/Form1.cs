﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.Xml.Serialization;
using System.IO;

namespace StartBZFS
{
    public partial class Form1 : Form
    {
        Prefrences prefs;
        DirectoryInfo confDir;

        ServerConfig serverConfig = new ServerConfig();

        public Form1()
        {
            InitializeComponent();
            loadPrefs();

            loadFormFromConfig(serverConfig);
        }

        private void loadFormFromConfig (ServerConfig config)
        {
            FFAMode.Checked = config.mode == GameMode.FFA;
            OFFAMode.Checked = config.mode == GameMode.OpenFFA;
            CTFMode.Checked = config.mode == GameMode.CTF;
            RabbitMode.Checked = config.mode == GameMode.Rabbit;

            PublicServer.Checked = config.publicServer;
            ServerAddress.Text = config.serverAddress;
            ServerPort.Text = config.port.ToString();
            checkAddressItem();
        }

        private void checkAddressItem()
        {
            ServerAddress.Enabled = PublicServer.Checked;
            ServerPort.Enabled = PublicServer.Checked;
            ServerTest.Enabled = PublicServer.Checked;
        }

        private void defaultPrefs ()
        {
            prefs = new Prefrences();
            MessageBox.Show("No BZFlag configuration has been found, please be sure to set your directory paths manualy");
        }

        private void savePrefs ()
        {
            // what the hell, try it
            if (!confDir.Exists)
                confDir.Create();
           
            FileInfo confFile = new FileInfo(Path.Combine(confDir.FullName, "StartBZFS.xml"));

            FileStream fs = confFile.OpenWrite();
            if (fs == null)
                return;

            XmlSerializer xml = new XmlSerializer(typeof(Prefrences));
            StreamWriter sr = new StreamWriter(fs);

            xml.Serialize(sr, prefs);
            sr.Close();
            fs.Close();
        }

        private void loadPrefs ( )
        {
            confDir = Prefrences.GetConfigDir();
            if (!confDir.Exists)
            {
                defaultPrefs();
                return;
            }

            FileInfo confFile = new FileInfo(Path.Combine(confDir.FullName, "StartBZFS.xml"));
            if (!confFile.Exists)
                prefs = new Prefrences();
            else
            {
                XmlSerializer xml = new XmlSerializer(typeof(Prefrences));
                FileStream fs = confFile.OpenRead();
                StreamReader sr = new StreamReader(fs);
                prefs = (Prefrences)xml.Deserialize(sr);
                sr.Close();
                fs.Close();
            }

            if (prefs.ClientPath == string.Empty || !new FileInfo(prefs.ClientPath).Exists)
            {
                FileInfo client = Prefrences.FindClient(confDir);
                if (client.Exists)
                    prefs.ClientPath = client.FullName;
                else
                    prefs.ClientPath = string.Empty;
            }
            
            // check for the server dir
            if (prefs.ServerPath == string.Empty || !new FileInfo(prefs.ServerPath).Exists)
            {
                FileInfo server = Prefrences.FindServer(confDir);
                if (server.Exists)
                    prefs.ServerPath = server.FullName;
                else
                    prefs.ServerPath = string.Empty;
            }

            // check for the world dir
            if (prefs.WorldPath == string.Empty || !new DirectoryInfo(prefs.WorldPath).Exists)
            {
                DirectoryInfo worldDir = Prefrences.FindWorldDir(confDir);
                if (worldDir.Exists)
                    prefs.WorldPath = worldDir.FullName;
                else
                    prefs.WorldPath = string.Empty;
            }
            
            if (prefs.ClientPath == string.Empty || prefs.ServerPath == string.Empty || prefs.WorldPath == string.Empty)
                MessageBox.Show("One or more of the paths to the BZFlag program files could not be found, please set them manualy");
        }

        private void pathsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (new Paths(confDir, prefs).ShowDialog() == DialogResult.OK)
                savePrefs();
        }

        private void PublicServer_CheckedChanged(object sender, EventArgs e)
        {
            ServerAddress.Enabled = PublicServer.Checked;
            ServerPort.Enabled = PublicServer.Checked;
            ServerTest.Enabled = PublicServer.Checked;
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            savePrefs();
        }
    }
}
