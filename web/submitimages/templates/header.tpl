<!DOCTYPE HTML PUBLIC '-//W3C//DTD HTML 4.01//EN' 'http://www.w3.org/TR/html4/strict.dtd'>
<html>
<head>
<title>{$config.site.title}{if $page.title} :: {$page.title}{/if}</title>
<link rel="stylesheet" type="text/css" href="bztheme.css">
{if $redirect}
<meta http-equiv="refresh" content="{$redirect.delay};URL={$redirect.url}">
{/if}
<meta http-equiv="content-type" content="text/html; charset=iso-8859-2">
<script type="text/javascript">
  var config_paths_baseURL = '{$config.paths.baseURL}';
  var config_upload_maxFiles = {$config.upload.maxFiles};
  var config_licenses = new Array();
{foreach from=$config.licenses item=license key=licenseid}
  config_licenses[{$licenseid}] = ['{$license.name}', '{$license.url}'];
{/foreach}
</script>
{foreach from=$page.javascripts item=javascript}
<script type="text/javascript" src="{$javascript}"></script>
{/foreach}

</head>
<body>

  <div id="header"><a href="{$config.paths.baseURL}"><img id="logo" src="logo.png" alt="{$config.site.title}"></a> 
    <ul id="navigation">
    <li><a href="{$config.paths.baseURL}">{$lang.home}</a></li>
{if !$user}
    <li><a href="http://my.bzflag.org/weblogin.php?action=weblogin&amp;url={$config.paths.baseURL|escape:'url'}login.php%3Ftoken%3D%25TOKEN%25%26username%3D%25USERNAME%25">{$lang.login}</a></li>
{else}
    <li><a href="{$config.paths.baseURL}upload.php">{$lang.uploadimages}</a></li>
{  if $user.moderator}
    <li><a href="{$config.paths.baseURL}queue.php">{$lang.moderationqueue}</a></li>
{  /if}
    <li><a href="{$config.paths.baseURL}logout.php">{$lang.logout}</a></li>
{/if}
    <li><a href="{$config.paths.baseURL}tos.php">{$lang.termsofservice}</a></li>
    </ul>
    <form action="{$config.paths.baseURL}setlanguage.php" method="get"><div>
      {$lang.setlanguage}: <select name="language">
{foreach from=$config.languages item=language}
        <option value="{$language.code}">{$language.name}</option>
{/foreach}
      </select>
      <input type="submit" value="Set">
    </div></form>
  </div>

  <div id="content">

