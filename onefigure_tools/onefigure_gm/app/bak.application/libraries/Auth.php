<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Auth {
    public function __construct($data = null) {
        $this->CI = & get_instance();
        $this->CI->load->library('session');

        if (isset($data['LEVEL']) && is_numeric($data['LEVEL'])) {
            $this->checkLevelRedirect($data['LEVEL']);
        }
    }

    public function getSession() {
        return $this->CI->session->userdata();
    }

    public function getManagerId() {
        return $this->CI->session->userdata('MANAGER_ID');
    }

    public function createSession($manager_id = null, $email = null, $name = null, $level = 0) {
        if (empty($manager_id) || empty($email) || empty($name) || !is_numeric($level)) return false;

        $data['MANAGER_ID'] = $manager_id;
        $data['EMAIL'] = $email;
        $data['NAME'] = $name;
        $data['LEVEL'] = $level;

        $this->CI->session->set_userdata($data);
    }

    public function updateSession() {
    }

    public function removeSession() {
        $data['EMAIL'] = '';
        $data['NAME'] = '';
        $data['LEVEL'] = '';

        $this->CI->session->set_userdata($data);
    }

    public function checkLevelRedirect($level) {
        if (!$this->CI->session->userdata('LEVEL')) {
            js_redirect(site_url(array('manager', 'login')));
        } else if ($this->CI->session->userdata('LEVEL') < $level) {
            js_alert_back('Permission error!');
        }
    }
}