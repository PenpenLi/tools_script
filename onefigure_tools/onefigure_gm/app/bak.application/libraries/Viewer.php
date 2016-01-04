<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Viewer {
    function all($view, $data = array()) {
        $CI =& get_instance();

        $CI->load->view('_common/common_header', $data);
        $CI->load->view('_common/full_header', $data);
        $CI->load->view($view, $data);
        $CI->load->view('_common/full_footer', $data);
        $CI->load->view('_common/common_footer', $data);
    }

    function popup($view, $data = array()) {
        $CI =& get_instance();

        $CI->load->view('_common/common_header', $data);
        $CI->load->view($view, $data);
        $CI->load->view('_common/common_footer', $data);
    }

    function login($view, $data = array()) {
        $CI =& get_instance();

        $CI->load->view('_common/login_header', $data);
        $CI->load->view($view, $data);
        $CI->load->view('_common/common_footer', $data);
    }
}
