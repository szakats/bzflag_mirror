<?php
  // List server rewrite by blast007 WiP
  // Started 6/4/2007
  
  /* Random thoughts and ideas
  - Country code for servers - a_temp_dist
  - < L4m3r> possibly a bit to choose whether or not other server owners are allowed to "use" the group
    So effective a non-sharable group. This would require the other part of L4m3r's idea
     * L4m3r thinks the list server should have an "owner" field for all listed servers
     This would also tie in with requiring a valid username/password for running public servers
     So, to summarize, it would be the following:
     - Username/password specified to run public servsers
     - Use this user's ID as the Owner ID of the server
     - Allow groups to have a non-sharable bit set, where the group can only be used if the owner of the group
        and owner of the server match     
  
  
  
  
  */
  
  include('config.php'); if (!defined('CONFIGURATION')) die("ERROR: Unable to load configuration.\n");
  
  include('functions.utility.php');
  include('functions.validation.php');

  // Use the same time throughout the whole duration of the script
  define('NOW', mktime());
 
  // Set up some headers right away
  header('Cache-Control: no-cache');
  header('Pragma: no-cache');
  header("Connection: close");
  // TODO: Should also send proper content-type (text/html or text/plain)
  // depending on if we're showing the bzfls web form or not
  header('Content-Type: text/plain');
  
  //////////////////////////////////////////
  
  // Read any user input, and determine the course of action to take
  
  // NOTE: In the previous incarnation of bzfls.php, slashes were added to input
  // and expected on input. In this version, they will be stripped away if the
  // input contains them, and only escaping user input when working with
  // database queries
  
  // The $_REQUEST variable contins the combination of the $_POST, $_GET, and
  // $_COOKIE variables. The order of precidence is based on the configuration
  // in php.ini
  // We use array_key_exists to check if they were specified, in case of a null
  // or empty value
  
  // Keep the global scope clean, and place all input into an array
  $input = Array();
  
  // TODO: Validate the input using validation functions
  // TODO: Make some validation functions
  
  // Determine which action we will be performing
  if (array_key_exists('action', $_REQUEST))
  {
    $input['action'] = strtoupper(smart_strip_slashes($_REQUEST['action']));
    
    // List of valid actions:
    // ADD - Request from bzfs to add a public server
    // REMOVE - Request from bzfs to remove a public server
    // LIST - Request from client to obtain a list of servers
    // GETTOKEN - Request from client to obtain an authentication token
    // CHECKTOKENS - Request from server to validate an authentication token
    // REGISTER - Request from client to register a callsign
    // CONFIRM - Request from browser(?) to confirm a callsign registration
    // NOTE: REGISTER and CONFIRM might be altered from the original
    //   implementation. Should not be a problem, since they aren't used anyway.
    
    if (!in_array($input['action'], Array('ADD', 'REMOVE', 'LIST', 'GETTOKEN', 'CHECKTOKENS', 'REGISTER', 'CONFIRM')))
    {
      die("ERROR: Invalid action specified. Insert magic HTML form here.\n");
    }
    
    
    // Now that we have the action, determine what other input we will need
    
    // nameport (ADD, REMOVE) - Public hostname/IP and port, in the form of
    //   name:port or name
    // NOTE: We won't use 'nameport' internally. This is simply used for 
    // compatability reasons with 2.0.x and 1.10.x servers
    if (array_key_exists('nameport', $_REQUEST) && ($input['action'] == 'ADD' || $input['action'] == 'REMOVE'))
    {
      $input['nameport'] = smart_strip_slashes($_REQUEST['nameport']);
      
      // Also populate the 'name' and 'port'
      
      // If there is not a colon, use port 5154
      if (strpos($input['nameport'], ':') === FALSE)
      {
        // No port was specified, so the whole 'nameport' is our name
        $input['name'] = $input['nameport'];
        // No port was specified, use 5154
        $input['port'] = 5154;
      }
      else
      {
        // Populate 'name' and 'port' by splitting 'nameport' by the colon
        list($input['name'], $input['port']) = explode(':', $input['nameport']);
      }
      
      // We don't need no nameport!
      unset($input['nameport']);
    }
    // If we didn't use 'nameport', see if we used 'name' and/or 'port'
    else
    {
      // name (ADD, REMOVE) - Public hostname/IP
      if (array_key_exists('name', $_REQUEST) && $input['action'] == 'ADD' || $input['action'] == 'REMOVE')
        $input['name'] = smart_strip_slashes($_REQUEST['name']);
      
      // port (ADD, REMOVE) - Public port
      if (array_key_exists('port', $_REQUEST) && ($input['action'] == 'ADD' || $input['action'] == 'REMOVE'))
        $input['port'] = smart_strip_slashes($_REQUEST['port']);
      // If no port was specified, and we're doing an ADD or REMOVE, use 5154
      else if ($input['action'] == 'ADD' || $input['action'] == 'REMOVE')
        $input['port'] = 5154;
    }

    // build (ADD) - Build string of server; Could also be used for clients
    if (array_key_exists('build', $_REQUEST) && ($input['action'] == 'ADD'))
      $input['build'] = smart_strip_slashes($_REQUEST['build']);
    
    // version (ADD, LIST) - Protocol string, specified to filter which
    //   servers are shown in a LIST, in the form BZFSxxxx, where xxxx is a
    //   number; 2.0.x uses BZFS0026, for example
    if (array_key_exists('version', $_REQUEST) && ($input['action'] == 'ADD' || $input['action'] == 'LIST'))
    {
      $input['version'] = smart_strip_slashes($_REQUEST['version']);
      
      if (!valid_version($input['version']))
      {
        die("ERROR: Invalid version specified. Must begin with 'BZFS' and be followed by a four digit, zero-padded number (e.g. BZFS0026)\n");
      }
    }
    
    // gameinfo (ADD) - Hex string that defines server configuration and
    //   player counts; Technically could be removed, and have bzfls.php
    //   query the server to obtain this information
    if (array_key_exists('gameinfo', $_REQUEST) && ($input['action'] == 'ADD'))
      $input['gameinfo'] = smart_strip_slashes($_REQUEST['gameinfo']);
    
    // title (ADD) - Public title/description of a server
    if (array_key_exists('title', $_REQUEST) && ($input['action'] == 'ADD'))
      $input['title'] = smart_strip_slashes($_REQUEST['title']);
    
    // checktokens (ADD, CHECKTOKENS) - Contains tokens that will be verified;
    //   These will be in the form callsign0=token or callsign0@ipaddress=token,
    //   and multiple instances of these can be seperated by \n
    if (array_key_exists('checktokens', $_REQUEST) && ($input['action'] == 'ADD' || $input['action'] == 'CHECKTOKENS'))
      $input['checktokens'] = smart_strip_slashes($_REQUEST['checktokens']);
    
    // groups (ADD, CHECKTOKENS) - List of groups to check for membership
    if (array_key_exists('groups', $_REQUEST) && ($input['action'] == 'ADD' || $input['action'] == 'CHECKTOKENS'))
      $input['groups'] = smart_strip_slashes($_REQUEST['groups']);
    
    // callsign (LIST, GETTOKEN, REGISTER, CONFIRM) - Callsign of player
    if (array_key_exists('callsign', $_REQUEST) && ($input['action'] == 'LIST' || $input['action'] == 'GETTOKEN' || $input['action'] == 'REGISTER' || $input['action'] == 'CONFIRM'))
    {
      $input['callsign'] = smart_strip_slashes($_REQUEST['callsign']);
      
      if (!isset($input['callsign']) || !valid_callsign($input['callsign']))
      {
        echo "ERROR: A valid callsign was not specified. Callsigns must use only ".
            "alphanumeric characters, hyphens, underscores, or periods. Must be ".
            "2 to 25 characters long.\n";
        if ($input['action'] !== 'LIST') exit;
      }
    }
    
    // email (REGISTER) - Email for registration
    if (array_key_exists('email', $_REQUEST) && ($input['action'] == 'REGISTER'))
      $input['email'] = smart_strip_slashes($_REQUEST['email']);
    
    // password (LIST, GETTOKEN, REGISTER) - User's password
    if (array_key_exists('password', $_REQUEST) && ($input['action'] == 'LIST' || $input['action'] == 'GETTOKEN' || $input['action'] == 'REGISTER'))
      $input['password'] = smart_strip_slashes($_REQUEST['password']);
    
    // advertgroups (ADD) - Specific groups to advertise the server to
    if (array_key_exists('advertgroups', $_REQUEST) && ($input['action'] == 'ADD'))
      $input['advertgroups'] = smart_strip_slashes($_REQUEST['advertgroups']);
    
    // activationkey (CONFIRM) - Randomly generated activation code sent via
    // email to the user.
    if (array_key_exists('activationkey', $_REQUEST) && ($input['action'] == 'CONFIRM'))
      $input['activationkey'] = smart_strip_slashes($_REQUEST['activationkey']);
  } // if (array_key_exists('action', $_REQUEST))
  
  // No action specified
  // TODO: Print HTML form thingy
  else
  {
    die("ERROR: No action specified. Insert magic HTML form here.\n");
  }
  
  
  //////////////////////////////////////////
  
  // Ready, Camera, ACTION!!
  
  //////////////////////////////////////////
  // ADD
  //////////////////////////////////////////
  if ($input['action'] == 'ADD')
  {
    // First off, check for some required input
  
    // We need to have a protocol version. If we do not, abort.
    if (!isset($input['version']) || empty($input['version']))
      die("ERROR: Protocol version not specified.\n");
      
    // We need to have the game info. If we do not, abort.
    if (!isset($input['gameinfo']) || empty($input['gameinfo']))
      die("ERROR: Game info not specified.\n");
      
    // Start off by copying all user input into our values to be passed
    $values = $input;
    
    // Next, see if we have a 'name' specified
    // Make sure it's not already a valid IP address
    if (isset($values['name'])) {
      // First, check if this already IS an IP address
      if (valid_IP_Address($values['name']))
      {
        $values['ipaddress'] = $values['name'];
      }
      // Was not an IP address. Try to resolve it.
      else
      {
        $values['ipaddress'] = gethostbyname($values['name']);
        
        // Unable to resolve the name, use the REMOTE_ADDR
        if ($values['ipaddress'] == $values['name'])
        {
          $values['name'] = $values['ipaddress'] = $_SERVER['REMOTE_ADDR'];
        }
      }
      
      // If the IP address equals 0.0.0.0, set it to the requesting address
      if ($values['ipaddress'] == '0.0.0.0')
      {
        $values['name'] = $values['ipaddress'] = $_SERVER['REMOTE_ADDR'];
      }
      
      // Verify that the IP address they specify is actually the one requesting
      // the ADD. This prevents a rogue user from altering list entries.
      else if (!$config['disableIPCheck'] && $values['ipaddress'] != $_SERVER['REMOTE_ADDR'])
      {
        die("ERROR: Specified IP did not match the requesting IP.\n");
      } 
    }
    // No name specified, use the request IP for 'name' and 'ipaddress'
    else
    {
      $values['name'] = $values['ipaddress'] = $_SERVER['REMOTE_ADDR'];
    }
    
    // Set up the lastmodified value
    $values['lastmodified'] = NOW;
    
    
    // Now that we have the IP address, see if this server is already in our
    // database.  If so, just update, otherwise, insert a new record
    
    // Load the DataLayer 
    @include_once('includes/datalayer.class.php');
    // Make sure the DataLayer class loaded sucessfully
    if (!class_exists('DataLayer')) die("ERROR: Unable to load DataLayer class.\n");
    $dl = new DataLayer($config['sql']['hostname'], $config['sql']['username'], $config['sql']['password'], $config['sql']['database']);
    if ($dl === false) die("ERROR: Unable to connect to database.\n");
    
    // Server already exists, update it
    if ($dl->Server_Exists_ByIPAddressPort($values) !== false)
    {
      $dl->Server_Update_ByIPAddressPort($values);
    }
    // Server doesn't exist, insert it
    else 
    {
      $dl->Server_Insert($values);
    }

  } // if ($input['action'] == 'ADD')
  
  
  // NOTE: The REMOVE action does not completely work when disableIPCheck is
  // enabled.  We do not resolve the name specified, so it might not always
  // match what is given. Minor, but should be fixed.
  else if ($input['action'] == 'REMOVE')
  {
    // Load the DataLayer 
    @include_once('includes/datalayer.class.php');
    // Make sure the DataLayer class loaded sucessfully
    if (!class_exists('DataLayer')) die("ERROR: Unable to load DataLayer class.\n");
    $dl = new DataLayer($config['sql']['hostname'], $config['sql']['username'], $config['sql']['password'], $config['sql']['database']);
    if ($dl === false) die("ERROR: Unable to connect to database.\n");
    
    // Values that will be passed to our DataLayer
    $values = Array();
       
    // We do not need to worry about the hostname. Just use the requesting
    // address.
    $values['ipaddress'] = $_SERVER['REMOTE_ADDR'];
    
    // Grab the port that was specified from our input
    $values['port'] = $input['port'];
    
    
    // Now that we have the IP address, see if this server is already in our
    // database.  If so, just update, otherwise, insert a new record
    
    // Remove the server
    $dl->Server_Delete_ByIPAddressPort($values);
  }
  
  // List servers
  else if ($input['action'] == 'LIST')
  {
    // Load the DataLayer 
    @include_once('includes/datalayer.class.php');
    // Make sure the DataLayer class loaded sucessfully
    if (!class_exists('DataLayer')) die("ERROR: Unable to load DataLayer class.\n");
    $dl = new DataLayer($config['sql']['hostname'], $config['sql']['username'], $config['sql']['password'], $config['sql']['database']);
    if ($dl === false) die("ERROR: Unable to connect to database.\n");
    
    // Prune out dead servers
    $dl->Servers_Delete_ByAge(NOW-$config['maximumServerAge']);
    
    // Are we filtering just for a specific version?
    if (isset($input['version']) && !empty($input['version']))
      $data['servers'] = $dl->Servers_Fetch_ByVersion($input['version']);
    // Else, show all the servers
    else
      $data['servers'] = $dl->Servers_Fetch_All();
    // name:port version gameinfo ipaddress title
    
    if ($data['servers'] && sizeof($data['servers']) > 0)
    {
      foreach($data['servers'] as $server)
        echo $server['name'].':'.$server['port'].' '.$server['version'].' '.$server['gameinfo'].' '.$server['ipaddress'].' '.$server['title']."\n";
    }
    
  }
  else if ($input['action'] == 'GETTOKEN')
  {
  
  }
  else if ($input['action'] == 'CHECKTOKENS')
  {
  
  }
  else if ($input['action'] == 'REGISTER')
  {
    if (!isset($input['callsign']) || !valid_callsign($input['callsign']))
    {
      die("ERROR: A valid callsign was not specified. Callsigns must use only ".
          "alphanumeric characters, hyphens, underscores, or periods. Must be ".
          "2 to 25 characters long.\n");
    }

    // TODO: Allow banning of email addresses with wildcard support
    if (!isset($input['email']) || !valid_email($input['email']))
    {
      die("ERROR: A valid email was not specified.\n");
    }
    
    if (!isset($input['password']) || !valid_password($input['password']))
    {
      die("ERROR: Password must be between 4 and 30 characters long.\n");
    }
    
    // Load the DataLayer 
    @include_once('includes/datalayer.class.php');
    // Make sure the DataLayer class loaded sucessfully
    if (!class_exists('DataLayer')) die("ERROR: Unable to load DataLayer class.\n");
    $dl = new DataLayer($config['sql']['hostname'], $config['sql']['username'], $config['sql']['password'], $config['sql']['database']);
    if ($dl === false) die("ERROR: Unable to connect to database.\n");
    
    
    $values = Array();
    $values['username'] = $input['callsign'];
    
    // Check if the user already exists before going further
    if ($dl->Player_Exists_ByUsername($values))
    {
      die("ERROR: The username '".$values['username']."' already exists. Please choose another username.\n");
    }
    else
    {
      // Leave the 'email' and 'password' blank. These will be filled after
      // the user activates their account
      $values['email'] = '';
      $values['password'] = '';
      
      $values['created'] = $values['lastaccess'] = NOW;
      $values['createdipaddress'] = $values['lastaccessipaddress'] = $_SERVER['REMOTE_ADDR'];
      
      // When the account is activated, these will be moved to 'password' and
      // 'email', respectively.
      $values['newpassword'] = md5($input['password']);
      $values['newemail'] = $input['email'];
      $values['activationkey'] = generate_random_string();
      $values['activated'] = 0;
      $values['token'] = '';
      $values['tokendate'] = 0;
      
      // Insert the player into the database
      $data['insertid'] = $dl->Player_Insert($values);
      
      if ($data['insertid'] !== false)
      {
        // Generate the nice welcome email for the user.
        $email = 
'Welcome to the BZFlag community! Your new account has been created.
Please keep this email for your records. Your account information is as follows:

Username: '.$values['username'].'
Password: '.$input['password'].'

Before you begin, you need to activate your account. Please follow the link below:
<a href="http://'.$_SERVER['SERVER_NAME'].$_SERVER['PHP_SELF'].'?action=CONFIRM&callsign='.urlencode($values['callsign']).'&activationkey='.urlencode($values['activationkey']).'">http://'.$_SERVER['SERVER_NAME'].$_SERVER['PHP_SELF'].'?action=CONFIRM&callsign='.urlencode($values['callsign']).'&activationkey='.urlencode($values['activationkey']).'</a>

If the above link does not work, copy and paste the following URL into your address bar:
http://'.$_SERVER['SERVER_NAME'].$_SERVER['PHP_SELF'].'?action=CONFIRM&callsign='.urlencode($values['username']).'&activationkey='.urlencode($values['activationkey']).'


Thank you for registering.';

        // TODO: Actually send the email. For now, we just echo it out.
        echo $email;

      }
      else
      {
        die("ERROR: There was a problem during registration. Please contact an administrator.\n");
      }
    }
  }
  else if ($input['action'] == 'CONFIRM')
  {    
    // Load the DataLayer 
    @include_once('includes/datalayer.class.php');
    // Make sure the DataLayer class loaded sucessfully
    if (!class_exists('DataLayer')) die("ERROR: Unable to load DataLayer class.\n");
    $dl = new DataLayer($config['sql']['hostname'], $config['sql']['username'], $config['sql']['password'], $config['sql']['database']);
    if ($dl === false) die("ERROR: Unable to connect to database.\n");
    
    $values = Array();
    $values['username'] = $input['callsign'];
    
    $data['player'] = $dl->Player_Fetch_ByUsername($values);
    
    if ($data['player'] !== false)
    {
      if ($input['activationkey'] == $data['player']['activationkey'])
      {
        if (isset($data['player']['newpassword']))
        {
          $data['player']['password'] = $data['player']['newpassword'];
          $data['player']['newpassword'] = '';
        }
        
        if (isset($data['player']['newemail']))
        {
          $data['player']['email'] = $data['player']['newemail'];
          $data['player']['newemail'] = '';
        }
        
        $data['player']['activationkey'] = '';
        $data['player']['activated'] = 1;
        
        $values['lastaccess'] = NOW;
        $values['lastaccessipaddress'] = $_SERVER['REMOTE_ADDR'];
        
        $data['updateplayer'] = $dl->Player_Update_ByUsername($data['player']);
        
        if ($data['updateplayer'])
        {
          die("NOTICE: Your account has been activated! Welcome to BZFlag!\n");
        }
        else
        {
          die("ERROR: There was an error activating your account. Please contact an administrator.\n");
        }
      }
      else
      {
        die("ERROR: The specified and activation key combination was not found in our database.\n");
      }
    }
    else
    {
      die("ERROR: The specified and activation key combination was not found in our database.\n");
    }
  }
  
?>
