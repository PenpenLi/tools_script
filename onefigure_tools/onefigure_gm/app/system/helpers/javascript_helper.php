<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

function js_body($script, $is_exit = true) {
    $body = '<script type="text/javascript">' . $script . '</script>';

    if ($is_exit) {
        die($body);
    } else {
        echo($body);
    }
}

function js_alert($message) {
    js_body('alert("' . $message . '");');
}

function js_alert_back($message) {
    js_body('alert("' . $message . '"); history.back();');
}

function js_alert_close($message) {
    js_body('alert("' . $message . '"); window.close();');
}

function js_alert_redirect($message, $url) {
    js_body('alert("' . $message . '"); location.href = "' . $url . '";');
}

function js_alert_action($message, $action) {
    js_body('alert("' . $message . '"); ' . $action);
}

function js_confirm_redirect($message, $url) {
    js_body('if (confirm("' . $message . '")) { location.href = "' . $url . '"; }');
}

function js_redirect($url) {
    js_body('location.href = "' . $url . '";');
}
