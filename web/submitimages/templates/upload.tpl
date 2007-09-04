<form enctype="multipart/form-data" action="{$config.paths.baseURL}upload.php" method="post" onsubmit="javascript:return validateForm();">
<input type="hidden" name="MAX_FILE_SIZE" value="{$config.upload.maxFileSize}">

<p>Currently only PNG files are supported. The maximum filesize per file is {$config.upload.maxFileSize} bytes, and you may upload {$config.upload.maxFiles} files at a time.</p>
<hr>

{section start=0 loop=$config.upload.maxFiles name=uploads}
<h2>File {$smarty.section.uploads.index_next}</h2>
<fieldset>
  <legend>File Information</legend>
  <label for="file{$smarty.section.uploads.index}">Image File: </label><input type="file" id="file{$smarty.section.uploads.index}" name="file[{$smarty.section.uploads.index}]">
</fieldset>
<fieldset>
  <legend>Author Information</legend>
  
  <label for="authorname{$smarty.section.uploads.index}">Author Name: </label><input type="text" id="authorname{$smarty.section.uploads.index}" name="authorname[{$smarty.section.uploads.index}]"><br>
  
  <label for="licenseselector{$smarty.section.uploads.index}">Select License: </label>
  <select id="licenseselector{$smarty.section.uploads.index}" name="licenseselector[{$smarty.section.uploads.index}]" onchange="javascript:handleLicenseSelector({$smarty.section.uploads.index});">
{foreach from=$config.licenses item=license key=licenseid}
    <option value="{$licenseid}"{if $licenseid == 0} selected="selected"{/if}>{$license.name}</option>
{/foreach}
  </select><input type="button" value="View" onclick="javascript:showLicense({$smarty.section.uploads.index});"><br>
  <span id="otherlicense{$smarty.section.uploads.index}" class="hide">
    <label for="otherlicensename{$smarty.section.uploads.index}">License Name: </label><input type="text" id="otherlicensename{$smarty.section.uploads.index}" name="otherlicensename[{$smarty.section.uploads.index}]"><br>
    Please specify a URL to the license and/or the license text itself.<br>
    <label for="otherlicenseurl{$smarty.section.uploads.index}">License URL: </label><input type="text" id="otherlicenseurl{$smarty.section.uploads.index}" name="otherlicenseurl[{$smarty.section.uploads.index}]"><br>
    <label for="otherlicensetext{$smarty.section.uploads.index}">License Text: </label><br><textarea style="width: 100%;" cols="74" rows="10" id="otherlicensetext{$smarty.section.uploads.index}" name="otherlicensetext[{$smarty.section.uploads.index}]"></textarea><br>
  </span>
  <input type="checkbox" id="confirm{$smarty.section.uploads.index}" name="confirm[{$smarty.section.uploads.index}]"><label id="confirmlabel{$smarty.section.uploads.index}" for="confirm{$smarty.section.uploads.index}"> I confirm that this image does not violate the <a href="{$config.baseURL}tos.php" onclick="javascript:return showTOS();">Terms Of Service</a>.</label>
</fieldset>
<hr>

{/section}

<h2>Other Information</h2>
<fieldset>
  <legend>Uploader Information</legend>
  <label>Username: </label><input type="text" disabled="disabled" value="{$user.username|escape:'htmlall'}"><br>
  <label for="uploaderfirstname">First Name: </label><input type="text" id="uploaderfirstname" name="uploaderfirstname"><br>
  <label for="uploaderfirstname">Last Name: </label><input type="text" id="uploaderlastname" name="uploaderlastname"><br>
  <input type="checkbox" id="confirmtos" name="confirmtos"><label id="confirmtoslabel" for="confirmtos"> I confirm that none of the above images violate the <a href="{$config.baseURL}tos.php" onclick="javascript:return showTOS();">Terms Of Service</a>.</label><br>
  <input type="checkbox" id="confirmaccurate" name="confirmaccurate"><label id="confirmaccuratelabel" for="confirmaccurate"> I confirm that all the information on this form is accurate.</label><br>
  <input type="submit" value="Upload Images">
</fieldset>
<hr id="errorhr" style="display: none;">
<fieldset id="erroroutput" style="display: none;"><legend>&nbsp;</legend></fieldset>
</form>