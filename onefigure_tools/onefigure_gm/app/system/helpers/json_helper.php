<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

function json_output($data = array(), $is_print = false) {
    if ($is_print) {
        //header('Content-Type:application/json');
        echo json_encode($data);
    } else {
        return json_encode($data);
    }
}
