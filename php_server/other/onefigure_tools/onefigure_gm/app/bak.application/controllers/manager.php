<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Manager extends CI_Controller {
    public function __construct() {
        parent::__construct();

        $this->load->model(array('Manager_data'));
        $this->load->library(array('Auth'));
        $this->load->helper(array('javascript', 'url', 'form', 'json'));
    }

    public function index() {
        $this->viewer->all('manager/list');
    }

    public function login() {
        $this->viewer->popup('manager/login');
    }

    public function logout() {
    }

    public function ajaxGetManager() {
        $data = array(
                'PAGE' => !$this->input->post('page') ? 1 : $this->input->post('page'),
                'LIMIT' => !$this->input->post('rows') ? 20 : $this->input->post('rows'),
            );

        $dataArray = $this->Manager_data->selectManagers($data);
        $managerRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        $response = array();
        $response['page'] = $data['PAGE'];
        $response['total'] = ceil($total_count / $data['LIMIT']);
        $response['records'] = $total_count;
        $response['rows'] = $managerRows;

        json_output($response, true);
    }

    public function ajaxSetManager() {
        if ($this->input->post('oper') == 'add') {
            $data = array(
                    'EMAIL' => $this->input->post('EMAIL'),
                    'PASSWORD' => $this->input->post('PASSWORD'),
                    'NAME' => $this->input->post('NAME'),
                    'LEVEL' => $this->input->post('LEVEL'),
                );

            $this->Manager_data->insertManager($data);
        } else if ($this->input->post('oper') == 'edit') {
            $data = array(
                    'MANAGER_ID' => $this->input->post('id'),
                    'EMAIL' => $this->input->post('EMAIL'),
                    'PASSWORD' => $this->input->post('PASSWORD'),
                    'NAME' => $this->input->post('NAME'),
                    'LEVEL' => $this->input->post('LEVEL'),
                );

            $this->Manager_data->updateManager($data);
        } else if ($this->input->post('oper') == 'del') {
            $data = array(
                    'MANAGER_ID' => $this->input->post('id')
                );

            $this->Manager_data->deleteManager($data);
        }
    }

    public function execSignUp() {
        $data = array(
            'EMAIL' => $this->input->post('email'),
            'PASSWORD' => $this->input->post('password'),
            'NAME' => $this->input->post('name'),
            'LEVEL' => 1,
        );

        $managerRow = $this->Manager_data->selectManager($data);

        if (isset($managerRow['MANAGER_ID'])) {
            js_alert_back('This email address already exists. Please try with another one.');
        }

        $manager_id = $this->Manager_data->insertManager($data);

        $this->auth->createSession($manager_id, $data['EMAIL'], $data['NAME'], $data['LEVEL']);
        js_redirect('/');
    }

    public function execLogin() {
        $data = array(
                'EMAIL' => $this->input->post('email'),
                'PASSWORD' => $this->input->post('password'),
            );

        $managerRow = $this->Manager_data->selectManager($data);

        if (!isset($managerRow['MANAGER_ID'])) {
            js_alert_back('Invalid member.');
        } else if ($managerRow['PASSWORD'] != md5($data['PASSWORD'])) {
            js_alert_back('You have entered an invalid password.');
        }

        $data = array(
                'MANAGER_ID' => $managerRow['MANAGER_ID'],
                'IS_LOGIN' => true,
            );

        $this->Manager_data->updateManager($data);
        
        $this->auth->createSession($managerRow['MANAGER_ID'], $managerRow['EMAIL'], $managerRow['NAME'], $managerRow['LEVEL']);
        js_redirect('/');
    }

    public function execLogout() {
        $this->auth->removeSession();
        js_redirect('/');
    }
}