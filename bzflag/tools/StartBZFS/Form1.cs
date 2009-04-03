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

        List<FileInfo> maps = new List<FileInfo>();

        bool settingMaps = false;

        int findMapIndex(string name)
        {
            int index = 0;
            foreach (FileInfo f in maps)
            {
                if (f.FullName == name)
                    return index;
                index++;
            }
            return -1;
        }

        public Form1()
        {
            InitializeComponent();
            loadPrefs();
            scanMaps();

            loadFormFromConfig(serverConfig);
        }

        private void scanMaps()
        {
            maps.Clear();

            DirectoryInfo mapDir = new DirectoryInfo(prefs.WorldPath);
            if (!mapDir.Exists)
                return;

            foreach (FileInfo f in mapDir.GetFiles("*.bzw"))
                maps.Add(f);
        }

        private void loadFormFromConfig (ServerConfig config)
        {
            FFAMode.Checked = config.mode == GameMode.FFA;
            OFFAMode.Checked = config.mode == GameMode.OpenFFA;
            CTFMode.Checked = config.mode == GameMode.CTF;
            RabbitMode.Checked = config.mode == GameMode.Rabbit;

            ServerAddress.Text = config.serverAddress;
            ServerPort.Text = config.port.ToString();

            NumShots.SelectedIndex = config.shots - 1;
            LogLevel.SelectedIndex = config.debugLevel;

            Ricochet.Checked = config.rico;
            Jumping.Checked = config.jumping;

            GoodFlags.Checked = config.goodFlags;
            BadFlags.Checked = config.badFlags;
            Antidote.Checked = config.andidote;
            FlagsOnBuildings.Checked = config.flagsOnBuildings;

            if (config.shakeWins > 0)
                ShakeWins.Text = config.shakeWins.ToString();
            if (config.shakeTime > 0)
                ShakeTime.Text = config.shakeTime.ToString();

            int index = 0;
            if (config.worldfile != string.Empty)
            {
                FileInfo f = new FileInfo(config.worldfile);
                if (!f.Exists)
                    config.worldfile = string.Empty;
                else
                {
                    index = findMapIndex(config.worldfile);
                    if (index < 0)
                    {
                        index = maps.Count;
                        maps.Add(f);
                    }
                }
            }
            buildWorldsList(index);

            Teleporters.Enabled = config.worldfile == string.Empty;
            Teleporters.Checked = config.teleporters;
            SpawnOnBoxes.Checked = config.spawnOnBoxes;

            checkAddressItem();
            checkServerStartButton();
        }

        private void setConfigFromForm (ServerConfig config)
        {
            if(FFAMode.Checked)
                config.mode = GameMode.FFA;
            else if(OFFAMode.Checked)
                config.mode = GameMode.OpenFFA;
            else if (CTFMode.Checked)
                config.mode = GameMode.CTF;
            else if(RabbitMode.Checked)
                config.mode = GameMode.Rabbit;

            config.shots = NumShots.SelectedIndex + 1;
            config.debugLevel = LogLevel.SelectedIndex;

            config.rico = Ricochet.Checked;
            config.jumping = Jumping.Checked;

            config.andidote = Antidote.Checked;
            config.flagsOnBuildings = FlagsOnBuildings.Checked;

            config.goodFlags = GoodFlags.Checked;
            config.badFlags = BadFlags.Checked;
            if (ShakeWins.Text != string.Empty)
            {
                int i = int.Parse(ShakeWins.Text);
                if ( i > 0)
                    config.shakeWins = i;
                else
                    config.shakeWins = 0;
            }

            if (ShakeTime.Text != string.Empty)
            {
                int i = int.Parse(ShakeWins.Text);
                if ( i > 0)
                    config.shakeTime = i;
                else
                    config.shakeTime = 0;
            }

            config.teleporters = Teleporters.Checked;
            config.spawnOnBoxes = SpawnOnBoxes.Checked;

            if (WorldsList.SelectedIndex == 0)
                config.worldfile = string.Empty;
            else
                config.worldfile = maps[WorldsList.SelectedIndex - 1].FullName;

            config.publicServer = PublicServer.Checked;
            if (config.publicServer)
            {
                config.serverAddress = ServerAddress.Text;

                if (ServerPort.Text == string.Empty)
                    config.port = 5154;
                else
                {
                    config.port = int.Parse(ServerPort.Text);
                    if (config.port < 1)
                        config.port = 5154;
                }
            }
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
            
            if (prefs.ServerPath == string.Empty || prefs.WorldPath == string.Empty)
                MessageBox.Show("One or more of the paths to the BZFlag program files could not be found, please set them manualy");
        }

        private void checkServerStartButton ()
        {
            bool enable = true;
            Status.Text = "Status:";

            if (prefs.ServerPath == string.Empty || !new FileInfo(prefs.ServerPath).Exists)
            {
                Status.Text += "No server path defined ";
                enable = false;
            }

            if (enable)
                Status.Text += "OK";
            Start.Enabled = enable;
        }

        private void buildWorldsList ( int index )
        {
            WorldsList.Items.Clear();
            WorldsList.Items.Add("Random");
            foreach(FileInfo i in maps)
                WorldsList.Items.Add(Path.GetFileNameWithoutExtension(i.FullName));

           WorldsList.Items.Add("Custom...");
           WorldsList.SelectedIndex = index;
        }

        private void pathsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (new Paths(confDir, prefs).ShowDialog() == DialogResult.OK)
            {
                savePrefs();
                scanMaps();
                buildWorldsList(0);
            }

            checkServerStartButton();
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

        private void Start_Click(object sender, EventArgs e)
        {
            setConfigFromForm(serverConfig);
            serverConfig.run(RunInBackground.Checked,prefs.ServerPath);
        }

        private void BadFlags_CheckedChanged(object sender, EventArgs e)
        {
            if (BadFlags.Checked)
            {
                if (ShakeTime.Text == string.Empty)
                    ShakeTime.Text = "30";
                if (ShakeWins.Text == string.Empty)
                    ShakeWins.Text = "1";
                Antidote.Checked = true;
            }
        }

        private void CTFMode_CheckedChanged(object sender, EventArgs e)
        {
            if (!FlagsOnBuildings.Checked)
                FlagsOnBuildings.Checked = CTFMode.Checked;
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (settingMaps)
                return;

            if (WorldsList.SelectedIndex == 0)
                Teleporters.Enabled = true;
            else
                Teleporters.Enabled = false;

            if (WorldsList.SelectedIndex > maps.Count)
            {
                OpenFileDialog ofd = new OpenFileDialog();
                ofd.CheckFileExists = true;
                ofd.Multiselect = false;
                ofd.FileName = "*bzw";
                ofd.InitialDirectory = prefs.WorldPath;
                ofd.Filter = "BZFlag Map files (*.bzw)|*.bzw|All files (*.*)|*.*";

                if (ofd.ShowDialog() == DialogResult.OK)
                {
                    settingMaps = true;
                    maps.Add(new FileInfo(ofd.FileName));
                    buildWorldsList(WorldsList.Items.Count-1);
                    settingMaps = false;
                }
            }
        }
    }
}
