<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Cache extends CI_Controller {
    public function __construct() {
        parent::__construct();
    }


    public function googleRefresh() {
        $this->google();
        $this->refresh();
    }

    public function refresh() {
        $this->load->driver('cache', array('adapter' => 'apc', 'backup' => 'file'));
        $this->cache->clean();

        echo json_encode(array('OK'));
    }

    public function google() {

        if (ENVIRONMENT != 'development') {
            die( 'DO NOT REQUEST THIS ON LiveServer');
        }

        $this->load->library(array('Curl'));
        $this->load->helper(array('file'));

        $limit = 60;

        // https://docs.google.com/spreadsheet/ccc?key=0AkEaZ70SHZbTdFN0Rmp4c21KbmRWQkkxMHVLNmZvY3c#gid=16
        //$googleDocsId = '0Apy5aHLUCTh6dFRoUkJIeTVuR09ZWFRYOHo5U3F2Q2c';
        $googleDocsId = '1QIPnYG2VaoFkrK3ZliaFhqmLeIyqPboiUdul9LFP-Qw';
        for ($i = 0; $i < $limit; $i++) {
            $docs['service'][$i] = 'https://docs.google.com/spreadsheet/pub?key='.$googleDocsId.'&single=true&gid=' . $i . '&output=csv';
        }

/*
        $temp = 201619001;
        $docs['service'][$temp] = 'https://docs.google.com/spreadsheet/pub?key='.$googleDocsId.'&single=true&gid=' . $temp . '&output=csv';

        $temp = 984786217;
        $docs['service'][$temp] = 'https://docs.google.com/spreadsheet/pub?key='.$googleDocsId.'&single=true&gid=' . $temp . '&output=csv';

        $temp = 77297536;
        $docs['service'][$temp] = 'https://docs.google.com/spreadsheet/pub?key='.$googleDocsId.'&single=true&gid=' . $temp . '&output=csv';

        $version = $this->uri->segment(1);
*/

        $files = array();
        foreach ($docs as $property => $value) {
            foreach ($value as $type => $url) {
                $this->curl->create($url);
                $contents = $this->curl->execute();

                $basefolder = '/home/nginx/html/images/Purple_OFDP/docs/ofdp/vlast';
                if (!file_exists($basefolder)) {
                    mkdir($basefolder, 0755, true);
                }

                if (!empty($contents) && substr(trim($contents), 0, 1) != '<') {
//                    $file = '/home/nginx/html/images/docs/ofdp/' . $property . '/' . $type . '.csv';
                    $file =  $basefolder . '/' . $type . '.csv';
                    write_file($file, $contents);
                    $files[] = $file;
                    echo $file."<br>";
                }
            }
        }

        //echo json_encode($files);
    }
}