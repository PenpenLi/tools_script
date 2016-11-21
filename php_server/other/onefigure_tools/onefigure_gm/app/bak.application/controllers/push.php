<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Push extends CI_Controller {
    function __construct() {
        parent::__construct();

        $this->load->model(array('Push_data'));
        $this->load->helper(array('javascript', 'url', 'form', 'json'));
    }

    function index() {
        $this->viewer->all('push/list');
    }

    function test() {
        $this->viewer->popup('push/test');
    }

    function sendiOSPushMessage() {
        $device_tokens = explode("\n", trim($this->input->post('device_token')));
        $message = trim($this->input->post('message'));
        $service = $this->input->post('service');
        $pem_file_path = $this->input->server('DOCUMENT_ROOT') . '/bin/';

        if ($service == 1) {
            $apnsHost = 'gateway.push.apple.com';
            $apnsCert = $pem_file_path . 'apns-product-DungeonHearts.pem';
        } else {
            $apnsHost = 'gateway.sandbox.push.apple.com';
            $apnsCert = $pem_file_path . 'apns_dev_devolver.pem';
        }

        $apnsPort = 2195;

        $payload = array('aps' => array('alert' => $message, 'badge' => 0, 'sound' => 'default'));
        $payload = json_encode($payload);

        $streamContext = stream_context_create();
        stream_context_set_option($streamContext, 'ssl', 'local_cert', $apnsCert);

        $apns = stream_socket_client('ssl://'.$apnsHost.':'.$apnsPort, $error, $errorString, 2, STREAM_CLIENT_CONNECT, $streamContext);

        if ($apns) {
            foreach ($device_tokens as $device_token) {
                $apnsMessage = chr(0).chr(0).chr(32).pack('H*', str_replace('%', '', $device_token)).chr(0).chr(strlen($payload)).$payload;
                fwrite($apns, $apnsMessage);
            }

            fclose($apns);

            js_alert_back('Send message success!');
        } else {
            js_alert_back('Server connect failed!');
        }
    }

    function ajaxGetMessage() {
        $data = array(
                'PAGE' => !$this->input->post('page') ? 1 : $this->input->post('page'),
                'LIMIT' => !$this->input->post('rows') ? 20 : $this->input->post('rows'),
            );

        $dataArray = $this->Push_data->selectPushMessageRows($data);
        $pushRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        $response = array();
        $response['page'] = $data['PAGE'];
        $response['total'] = ceil($total_count / $data['LIMIT']);
        $response['records'] = $total_count;
        foreach ($pushRows as $row) {
            $response['rows'][] = $row;
        }

        json_output($response, true);
    }

    function ajaxSetMessage() {
        if ($this->input->post('oper') == 'add') {
            $data = array(
                    'SEND_DATE' => $this->input->post('SEND_DATE'),
                    'SEND_TIME' => $this->input->post('SEND_TIME'),
                    'OS_TYPE' => $this->input->post('OS_TYPE'),
                    'TITLE' => $this->input->post('TITLE'),
                    'MESSAGE' => $this->input->post('MESSAGE'),
                    'USER_CODE' => $this->input->post('USER_CODE'),
                );

            $this->Push_data->insertPushMessageRow($data);
        } else if ($this->input->post('oper') == 'edit') {
            $data = array(
                    'MESSAGE_ID' => $this->input->post('id'),
                    'SEND_DATE' => $this->input->post('SEND_DATE'),
                    'SEND_TIME' => $this->input->post('SEND_TIME'),
                    'OS_TYPE' => $this->input->post('OS_TYPE'),
                    'TITLE' => $this->input->post('TITLE'),
                    'MESSAGE' => $this->input->post('MESSAGE'),
                    'USER_CODE' => $this->input->post('USER_CODE'),
                );

            $this->Push_data->updatePushMessageRow($data);
        } else if ($this->input->post('oper') == 'del') {
            $data = array(
                    'MESSAGE_ID' => $this->input->post('id'),
                );

            $this->Push_data->deletePushMessageRow($data);
        }
    }
}
