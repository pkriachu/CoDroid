<?php

function getline( $fp, $delim )
{
    $result = "";
    while( !feof( $fp ) )
    {
        $tmp = fgetc( $fp );
        if( $tmp == $delim )
            return $result;
        $result .= $tmp;
    }
    return $result;
}

?>