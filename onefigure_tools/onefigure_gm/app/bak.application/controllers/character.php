<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Character extends CI_Controller {
    function __construct() {
        parent::__construct();

        $this->load->model(array('facebook/Character_data'));
        $this->load->helper(array('json'));
    }

    function ajaxGetCharactersOfUserData($user_id) {
        $data = array(
                'USER_ID' => $user_id,
                'PAGE' => !$this->input->post('page') ? 1 : $this->input->post('page'),
                'LIMIT' => !$this->input->post('rows') ? 20 : $this->input->post('rows'),
            );

        $dataArray = $this->Character_data->getCharacterRows($data);
        $characterRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        $response = array();
        $response['page'] = $data['PAGE'];
        $response['total'] = ceil($total_count / $data['LIMIT']);
        $response['records'] = $total_count;
        $response['rows'] = $characterRows;

        json_output($response, true);
    }
}