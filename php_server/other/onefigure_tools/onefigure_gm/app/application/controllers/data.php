<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Data extends CI_Controller {
    public function __construct() {
        parent::__construct();

        $this->load->helper(array('file'));

        $this->load->library('Auth', array('LEVEL' => 1));
    }

    public function index() {
        $data = array(
            'HOSTS' => read_file('/tmp/ofdp_web_servers')
        );

        $this->viewer->all('data/index', $data);
    }

    public function ajaxRefreshCache() {
        $this->load->driver('cache', array('adapter' => 'apc', 'backup' => 'file'));
        $this->cache->clean();

        $this->load->library(array('Curl'));

        $hosts = $this->input->post('hosts');
        write_file('/tmp/ofdp_web_servers', $hosts);

        foreach (explode("\n", $hosts) as $host) {
            $url = 'http://' . trim($host) . '/v1/cache/refresh';

            $this->curl->create($url);
            $contents = $this->curl->execute();
        }

        echo 'OK';
    }

    public function ajaxRefreshDocs() {
        $this->load->library(array('Curl'));

        $hosts = $this->input->post('hosts');
        write_file('/tmp/ofdp_web_servers', $hosts);

        foreach (explode("\n", $hosts) as $host) {
            $url = 'http://' . trim($host) . '/v1/cache/google';

            $this->curl->create($url);
            $contents = $this->curl->execute();
        }

        echo 'OK';
    }
}
