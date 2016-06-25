function fileSelected() {
    var file = document.getElementById('fileToUpload').files[0];
	
    if ( file ) {
        extIndex = file.name.lastIndexOf('.');
        if ( extIndex != -1 ) {
			//parse filename extension
            extName = file.name.substr(extIndex+1, file.name.length);
            extName = extName.toLowerCase();

			//display file name
			$('#filename').val(file.name);

			//check apk file
            if ( extName==="apk" ) {
                //unlock upload button
				$('#upload').attr( 'class', 'link-button' );
				$('#upload').attr( 'onclick', 'document.forms["uploadForm"].startUpload.click();' );
            } else {
				//lock upload button
				$('#upload').attr( 'class', 'link-button-locked' );
				$('#upload').unbind();
                alert('Please input a .apk file.');
            }
        }
    }
}



function preSelect() {
    $('#fileToUpload').val('');
	$('#filename').val('');
	$('#fileToUpload').click();
}
