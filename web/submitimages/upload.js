function $(id) {
  if (typeof id == 'string')
  {
    if (document.getElementById)
      return document.getElementById(id);
    else
      return false;
  }
  
  return false;
}


function handleLicenseSelector(uploadIndex)
{
  var selected = $('licenseselector'+uploadIndex).value;
  
  // If they did not choose "Other OSI-Approved", just show them the license
  if (selected != 255)
  {
    $('otherlicense'+uploadIndex).className = 'hide';
    $('otherlicensename'+uploadIndex).disabled = true;
    $('otherlicenseurl'+uploadIndex).disabled = true;
    $('otherlicensetext'+uploadIndex).disabled = true;
  }
  else
  {
    $('otherlicensename'+uploadIndex).disabled = false;
    $('otherlicenseurl'+uploadIndex).disabled = false;
    $('otherlicensetext'+uploadIndex).disabled = false;
    $('otherlicense'+uploadIndex).className = '';
  }
}

function showLicense(uploadIndex)
{
  var selected = $('licenseselector'+uploadIndex).value;
  
  if (!config_licenses || !config_licenses[selected]) return;
  
  var licensepopup = window.open(config_licenses[selected][1], 'licensepopup');
  licensepopup.focus();
}

function showTOS()
{  
  var tospopup = window.open(config_paths_baseURL+'tos.php', 'tos');
  if (!tospopup) return true;
  tospopup.focus();
  return false;
}

function validateForm()
{
  // If there was an error, this will be set to true. We won't bail out of the
  // script until everything was validated so that we can highlight every error
  var isError = false;
  
  // Only show the permission page once
  // This is triggered when they have "I stole this" for an image license
  var shownPermissionPage = false;
  
  // Keep track of how many images the user wants to upload
  var imageUploads = 0;
  
  // Check each upload
  for (var i = 0; i < config_upload_maxFiles; i++)
  {
    var file = $('file'+i);
    if (file.value.length > 0)
    {
      // They filled in the file upload box, so increment our counter
      imageUploads++;
      
      // TODO: Look for a .png extension
      
      // Verify they filled in an author field
      var authorname = $('authorname'+i);
      // Length is 0, they didn't fill in a value
      if (authorname.value.length == 0) {
        isError = true;
        authorname.className = 'invalid';
      }
      else authorname.className = '';
      
      // Do some checks on the license selector.
      // A value of 0 == "I stole this"
      // A value of 255 == "Other OSI-Approved"
      var licenseselector = $('licenseselector'+i);
      if (licenseselector.value == 0)
      {
        isError = true;
        licenseselector.className = 'invalid';
        // Open a popup, but only once per validation
        if (!shownPermissionPage) {
          var istolethis = window.open(config_paths_baseURL+'permission.php', 'istolethis');
          if (istolethis) {
            shownPermissionPage = true;
            istolethis.focus();
          }
        }
      } // if (licenseselector.value == 0)
      else if (licenseselector.value == 255)
      {
        var otherlicensename = $('otherlicensename'+i);
        var otherlicenseurl = $('otherlicenseurl'+i);
        var otherlicensetext = $('otherlicensetext'+i);
        
        // Assume they provide a license name, and either a license url or
        // license text
        licenseselector.className = '';
        
        // If there is no license name, mark the selector and the name invalid
        if (otherlicensename.value.length == 0)
        {
          isError = true;
          licenseselector.className = 'invalid';
          otherlicensename.className = 'invalid';
        }
        else otherlicensename.className = '';
        
        // If there is no license name, mark the selector and the name invalid
        if (otherlicensename.value.length == 0)
        {
          isError = true;
          licenseselector.className = 'invalid';
          otherlicensename.className = 'invalid';
        }
        else otherlicensename.className = '';
        
        
        // If there is no license url or text, mark the selector and the url and
        // text fields invalid
        if (otherlicenseurl.value.length == 0 && otherlicensetext.value.length == 0)
        {
          isError = true;
          licenseselector.className = 'invalid';
          otherlicenseurl.className = 'invalid';
          otherlicensetext.className = 'invalid';
        }
        else
        {
          otherlicenseurl.className = '';
          otherlicensetext.className = '';
        }
      } // else if (licenseselector.value == 255)
      else licenseselector.className = '';
      
      // They must also a agree to the TOS
      var agree = $('agree'+i);
      if (!agree.checked)
      {
        isError = true;
        $('agreelabel'+i).className = 'invalid';
      }
      else $('agreelabel'+i).className = '';
        
    } // if (file.value.length > 0)
  }
  
  // If the user didn't fill in any of the upload boxes, we have nothing to do
  if (imageUploads == 0)
  {
    alert("Error: You did not choose to upload any files.");
    isError = true;
  } 
  
  // Check uploader portion
  
  
  if (isError) alert("One or more errors were detected. Please correct them and try again.");

  return !isError;
}
