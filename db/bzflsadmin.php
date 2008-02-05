<?php
// bzlogin.php
//
// Copyright (c) 1993 - 2004 Tim Riker
//
// This package is free software;you can redistribute it and/or
// modify it under the terms of the license found in the file
// named COPYING that should have accompanied this file.
//
// THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
// WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.

// where to send debug printing (might override below)
$enableDebug= 0;
$debugFile= 'bzfls.log';

// define dbhost/dbuname/dbpass/dbname here
// NOTE it's .php so folks can't read the source
include ('/etc/bzflag/serversettings.php');

// start the session
session_start();

// connect to MySQL
$link = @mysql_connect ($dbhost, $dbuname, $dbpass) or die ("Could not connect to MySQL.");

// main action logic switch
switch ($_REQUEST['action']) {
case 'LOGIN':
	mysql_select_db ($bbdbname) or die ("Could not select user database.");
	$sql = sprintf ('SELECT user_id FROM phpbb_users WHERE username = "%s" AND user_password = MD5("%s")',
			mysql_real_escape_string ($_POST['username']), mysql_real_escape_string ($_POST['password']));
	$result = mysql_query ($sql);

	// check for valid result and valid login
	if (! $result) {
		dumpPageHeader();
		echo 'Sorry, unknown error: <div style="display: inline; color: grey">'.mysql_error().'</div>';
		dumpPageFooter();

		break;
	}
	if (mysql_num_rows ($result) < 1) {
		dumpPageHeader();
		echo 'Sorry, could not log you in with the specified credentials.';
		dumpPageFooter();

		break;
	}

	// get the bzid and put it in the session var
	$_SESSION['bzid'] = mysql_result ($result, 0, "user_id");

	// check that this user is a list server admin
	$sql = 'SELECT group_id FROM phpbb_user_group WHERE user_id = '.$_SESSION['bzid'].' AND group_id = '.
			'(SELECT group_id FROM phpbb_groups WHERE group_name = "developers")';
	$result = mysql_query ($sql);
	if (! $result) {
		dumpPageHeader();
		echo 'Sorry, unknown error: <div style="display: inline; color: grey">'.mysql_error().'</div>';
		dumpPageFooter();

		break;
	}
	if (mysql_num_rows ($result) < 1) {
		unset ($_SESSION['bzid']);
		dumpPageHeader();
		echo 'Sorry, you are not allowed to administer the list server.';
		dumpPageFooter();

		break;
	}

	// if we got here, the user is good to go; redirect
	header ("Location: http://".$_SERVER['HTTP_HOST'].$_SERVER['PHP_SELF']);

	break;
case 'ACTIVATE':
case 'DEACTIVATE':
	// check auth
	if (! $_SESSION['bzid']) {
		dumpMainPage();

		break;
	}

	// make sure we have a target
	if (! $_POST['id']) {
		header ("Location: http://".$_SERVER['HTTP_HOST'].$_SERVER['PHP_SELF']);

		break;
	}

	// make the update
	mysql_select_db ($dbname) or die ("Could not select bzfls database.");
	$sql = sprintf ('UPDATE serverbans SET active = %u WHERE banid = %u',
			($_REQUEST['action'] == ACTIVATE ? 1 : 0), $_POST['id']);
	$result = mysql_query ($sql);
	if (! $result) {
		dumpPageHeader();
		echo 'Sorry, unknown error: <div style="display: inline; color: grey">'.mysql_error().'</div>';
		dumpPageFooter();

		break;
	}

	header ("Location: http://".$_SERVER['HTTP_HOST'].$_SERVER['PHP_SELF']);

	break;
case 'NEW':
case 'EDIT':
	// check auth
	if (! $_SESSION['bzid']) {
		dumpMainPage();

		break;
	}

	// get the original data if it exists
	$data = array();
	mysql_select_db ($dbname) or die ("Could not select bzfls database.");
	$sql = sprintf ('SELECT * FROM serverbans WHERE banid = %u', $_POST['id']);
	$result = mysql_query ($sql);

	if ($result && mysql_num_rows ($result) > 0)
		$data = mysql_fetch_array ($result);

	dumpPageHeader();
	?>
<b><?php echo  ($_REQUEST['action'] == "NEW" ? "New" : "Edit")." Ban"; ?></b><br>
<form method="POST" action="<?php echo $_SERVER['PHP_SELF']; ?>">
	<input type="hidden" name="action" value="UPDATE">
	<input type="hidden" name="id" value="<?php echo $_POST['id']; ?>">
	<table class="listform">
		<tr><td>IP/Hostname</td><td><input type="text" name="address" value="<?php echo $data['address']; ?>"></td></tr>
		<tr><td>Owner</td><td><input type="text" name="owner" value="<?php echo $data['owner']; ?>"></td></tr>
		<tr><td>Reason</td><td><input type="text" name="reason" value="<?php echo $data['reason']; ?>"></td></tr>
	</table>
	<br>

	<input type="submit" value="Done">
</form>
<?php
	dumpPageFooter();

	break;
case 'UPDATE':
	// check auth
	if (! $_SESSION['bzid']) {
		dumpMainPage();

		break;
	}

	// make sure we have data
	if (! $_POST['address']) {
		header ("Location: http://".$_SERVER['HTTP_HOST'].$_SERVER['PHP_SELF']);

		break;
	}

	// run update query
	mysql_select_db ($dbname) or die ("Could not select bzbb database.");
	if ($_POST['id'])
		$sql = sprintf ('UPDATE serverbans SET address = "%s", owner = "%s", reason = "%s", lastby = %u WHERE banid = %u',
				mysql_real_escape_string ($_POST['address']),
				mysql_real_escape_string ($_POST['owner']),
				mysql_real_escape_string ($_POST['reason']),
				$_SESSION['bzid'],
				$_POST['id']);
	else
		$sql = sprintf ('INSERT INTO serverbans SET address = "%s", owner = "%s", reason = "%s", lastby = %u',
				mysql_real_escape_string ($_POST['address']),
				mysql_real_escape_string ($_POST['owner']),
				mysql_real_escape_string ($_POST['reason']),
				$_SESSION['bzid']);
	$result = mysql_query ($sql);
	if (! $result) {
		dumpPageHeader();
		echo 'Sorry, unknown error: <div style="display: inline; color: grey">'.mysql_error().'</div>';
		dumpPageFooter();

		break;
	}

	header ("Location: http://".$_SERVER['HTTP_HOST'].$_SERVER['PHP_SELF']);

	break;
case 'DELETE':
	// check auth
	if (! $_SESSION['bzid']) {
		dumpMainPage();

		break;
	}

	// make sure we have a target
	if (! $_POST['id']) {
		header ("Location: http://".$_SERVER['HTTP_HOST'].$_SERVER['PHP_SELF']);

		break;
	}

	// run deletion query
	mysql_select_db ($dbname) or die ("Could not select bzbb database.");
	$sql = sprintf ('DELETE FROM serverbans WHERE banid = %u', $_POST['id']);
	$result = mysql_query ($sql);

	if (! $result) {
		dumpPageHeader();
		echo 'Sorry, unknown error: <div style="display: inline; color: grey">'.mysql_error().'</div>';
		dumpPageFooter();

		break;
	}

	header ("Location: http://".$_SERVER['HTTP_HOST'].$_SERVER['PHP_SELF']);

	break;
default:
	dumpMainPage();

	break;
}

// functions
function dumpMainPage() {
	global $link, $dbname, $bbdbname;

	dumpPageHeader();

	if (! $_SESSION['bzid']) {
		// We're not logged in... print login form

		?>
This page is the admin interface for the BZFlag list server located at my.bzflag.org. If you are a list server administrator, please log in. Otherwise, please disconnect now.<br><br>

<form method="POST" action="<?php echo $_SERVER['PHP_SELF']; ?>">
	<input type="hidden" name="action" value="LOGIN">
	<table>
		<tr><td>Username:</td><td><input type="text" name="username" size="20"></td></tr>
		<tr><td>Password:</td><td><input type="password" name="password" size="20"></td></tr>
	</table>
	<input type="submit" value="Log In">
</form>

<?php
	}

	// user is logged in... print main admin page, starting with welcome
	mysql_select_db ($bbdbname) or die ("Could not select user database.");
	$sql = 'SELECT username FROM phpbb_users WHERE user_id = '.$_SESSION['bzid'];
	$result = mysql_query ($sql);

	if (! $result) {
		echo 'Sorry, unknown error: <div style="display: inline; color: grey">'.mysql_error().'</div>';
		dumpPageFooter();

		return;
	} else if (mysql_num_rows ($result) > 0) {
		echo '<i>Wassup, '.mysql_result ($result, 0, "username")."?</i><br><br>\n\n";
	}

	// current bans list
	mysql_select_db ($dbname) or die ("Could not select bzfls database.");
	$sql = 'SELECT * FROM serverbans WHERE 1';
	$result = mysql_query ($sql);
	if (! $result) {
		echo 'Sorry, unknown error: <div style="display: inline; color: grey">'.mysql_error().'</div>';
		dumpPageFooter();

		return;
	}

	echo "<b>Bans</b><br>\n";

	if (mysql_num_rows ($result) > 0) {
		?>
<table cellpadding="5px" class="listform" border=1>
	<tr class="dark">
		<td>Active</td>
		<td>IP/Hostname</td>
		<td>Owner</td>
		<td>Reason</td>
		<td>By</td>
		<td colspan="3">&nbsp;</td>
	</tr>
<?php
		// compile array of current bans
		$bans = array();
		while ($result_array = mysql_fetch_array ($result))
			array_push ($bans, array(
			'id' => $result_array['banid'],
			'active' => $result_array['active'],
			'address' => $result_array['address'],
			'owner' => $result_array['owner'],
			'reason' => $result_array['reason'],
			'lastby' => $result_array['lastby']));
 
		// convert each 'lastby' bzid to a username
		mysql_select_db ($bbdbname) or die ("Could not select user database.");
		for ($i = 0; $i < count ($bans); ++$i) {
			$sql = 'SELECT username FROM phpbb_users WHERE user_id = '.$bans[$i]['lastby'];
			$result = mysql_query ($sql);
			if ($result && mysql_num_rows ($result) > 0)
				$bans[$i]['lastby'] = mysql_result ($result, 0, "username");
		}

		// output the row
		foreach ($bans as $ban) {
			echo '<tr>'.
					'<td>'. ($ban['active'] ? 'Yes' : 'No').'</td>'.
					'<td>'.$ban['address'].'</td>'.
					'<td>'.$ban['owner'].'</td>'.
					'<td>'.$ban['reason'].'</td>'.
					'<td>'.$ban['lastby'].'</td>'.
					'<td align="center">'.'<form method="POST" action="'.$_SERVER['PHP_SELF'].'">'.
							'<input type="hidden" name="action" value="'. ($ban['active'] ? "DEACTIVATE" : "ACTIVATE").'">'.
							'<input type="hidden" name="id" value="'.$ban['id'].'">'.
							'<input type="submit" value="'. ($ban['active'] ? "Deactivate" : "Activate").'">'.
							'</form></td>'.
					'<td align="center"><form method="POST" action="'.$_SERVER['PHP_SELF'].'">'.
							'<input type="hidden" name="action" value="EDIT">'.
							'<input type="hidden" name="id" value="'.$ban['id'].'">'.
							'<input type="submit" value="Edit"></form></td>'.
					'<td align="center"><form method="POST" action="'.$_SERVER['PHP_SELF'].'">'.
							'<input type="hidden" name="action" value="DELETE">'.
							'<input type="hidden" name="id" value="'.$ban['id'].'">'.
							'<input type="submit" value="Delete"></form></td>'.
					"</tr>\n";
		}

		?>
</table>
<br>
<?php
	} else {
		echo "<i>There are no bans on file at this time.</i><br><br>\n\n";
	}

	?>
<form method="POST" action="<?php echo $_SERVER['PHP_SELF']; ?>">
	<input type="hidden" name="action" value="NEW">
	<input type="submit" value="New Ban">
</form>
<?php
	dumpPageFooter();
}

function dumpPageHeader () {
	# tell the proxies not to cache
	header('Cache-Control: no-cache');
	header('Pragma: no-cache');
	header('Content-type: text/html');

	print (
'<HTML>
	<head>
		<title>BZFlag List Server Administration</title>
		<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
		<link rel="stylesheet" type="text/css" href="http://www.bzflag.org/general.css">
		<link href="http://www.bzflag.or/favicon.ico" rel="shortcut icon">
		<style type="text/css">
			table.listform {
				border-collapse: collapse;
			}
			table.listform td {
				font-family: sans-serif;
				font-size: 10pt;
				vertical-align: center;
			}
			tr.dark td {
				background-color: #808080;
			}
		</style>
	</head>
	<BODY>
		<table width="100%" border="0" cellpadding="0" cellspacing="1" bgcolor="#DDDDDD">
			<tr height="50" valign="top">
				<td colspan="2">
					<table border="0" cellpadding="0" cellspacing="0" width="100%">
						<tr>
							<td bgcolor="#013571" align="right"><img src="http://www.bzflag.org/images/logo2-1.jpg" alt="logo"></td>
							<td bgcolor="#818181" align="left"><img src="http://www.bzflag.org/images/logo2-2.jpg" alt=""></td>
						</tr>
					</table>
				</td>
			</tr>
			<tr valign="top">
				<td>
					<table width="100%" border="0" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
						<tr>
							<td valign="top"><br><center><b>BZFlag List Server Administration</b></center><br></td>
						</tr>
						<tr>
							<td>
'
		);
}

function dumpPageFooter () {
	print(
'							</td>
						</tr>
					</table>
				</td>
			</tr>
			<tr valign="bottom">
				<td bgcolor="#000000" cellpadding="2">
					<table width="100%" border="0" cellpadding="2" bgcolor="#FFFFFF">
						<tr>
							<td align="right">
								<span class="copyright">copyright &copy; <a href="http://www.bzflag.org/wiki/CurrentMaintainer">CurrentMaintainer</a> 1993-2008&nbsp;</span>
							</td>
						</tr>
					</table>
				</td>
			</tr>
		</table>
	</BODY>
</HTML>'
		);
}


?>
