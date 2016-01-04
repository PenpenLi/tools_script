<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Event extends CI_Controller {
    function __construct() {
        parent::__construct();

        $this->load->model(array('Event_data'));
        $this->load->helper(array('javascript', 'url', 'form', 'json'));

        $this->load->library('Auth', array('LEVEL' => 1));
    }

    public function index() {
        $this->all();
    }

    public function all($page = 1) {
        $data = array(
            'PAGE' => $page,
            'LIMIT' => 10,
        );

        $dataArray = $this->Event_data->selectEvents($data);
        $data['EVENT_ROWS'] = $dataArray['DATA'];
        $data['EVENT_TOTAL_COUNT'] = $dataArray['TOTAL_COUNT'];
        $data['EVENT_TOTAL_PAGE'] = ceil($data['EVENT_TOTAL_COUNT'] / $data['LIMIT']);

        $this->viewer->all('event/list', $data);
    }

    public function edit($event_id = null) {
        $data = array('EVENT_ID' => null, 'EVENT_ROW' => null);
        if ($event_id > 0) {
            $data['EVENT_ID'] = $event_id;
            $data['EVENT_ROW'] = $this->Event_data->selectEvent($data);
        }

        $this->viewer->all('event/edit', $data);
    }

    public function execSetEvent() {
        if (!$this->input->post('event_id')) {
            $data = array(
                'PROCESS_CODE' => $this->input->post('process'),
                'TITLE' => $this->input->post('title'),
                'START_DATE' => $this->input->post('start_date'),
                'END_DATE' => $this->input->post('end_date'),
                'REPEAT_CODE' => $this->input->post('repeat_type'),
                'REPEAT_COUNT' => $this->input->post('repeat_count'),
                'LIMIT_COUNT' => $this->input->post('repeat_limit'),
                'GIFT_TYPE' => $this->input->post('reward_type'),
                'ITEM_ID' => $this->input->post('reward_item_id'),
                'AMOUNT' => $this->input->post('reward_amount'),
                'SPOT_START_TIME' => $this->input->post('spot_start_time'),
                'SPOT_END_TIME' => $this->input->post('spot_end_time'),
            );

            $this->Event_data->insertEvent($data);
        } else {
            $data = array(
                'EVENT_ID' => $this->input->post('event_id'),
                'PROCESS_CODE' => $this->input->post('process'),
                'TITLE' => $this->input->post('title'),
                'START_DATE' => $this->input->post('start_date'),
                'END_DATE' => $this->input->post('end_date'),
                'REPEAT_CODE' => $this->input->post('repeat_type'),
                'REPEAT_COUNT' => $this->input->post('repeat_count'),
                'LIMIT_COUNT' => $this->input->post('repeat_limit'),
                'GIFT_TYPE' => $this->input->post('reward_type'),
                'ITEM_ID' => $this->input->post('reward_item_id'),
                'AMOUNT' => $this->input->post('reward_amount'),
                'SPOT_START_TIME' => $this->input->post('spot_start_time'),
                'SPOT_END_TIME' => $this->input->post('spot_end_time'),
            );

            $this->Event_data->updateEvent($data);
        }

        js_alert_redirect('Done.', site_url(array('event')));
    }

    public function ajaxGetEvent() {
        $data = array(
                'PAGE' => !$this->input->post('page') ? 1 : $this->input->post('page'),
                'LIMIT' => !$this->input->post('rows') ? 20 : $this->input->post('rows'),
            );

        $dataArray = $this->Event_data->selectEventRows($data);
        $eventRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        $response = array();
        $response['page'] = $data['PAGE'];
        $response['total'] = ceil($total_count / $data['LIMIT']);
        $response['records'] = $total_count;
        $response['rows'] = $eventRows;

        json_output($response, true);
    }

    function ajaxSetEvent() {
        if ($this->input->post('oper') == 'add') {
            $data = array(
                    'PROCESS_CODE' => $this->input->post('PROCESS_CODE'),
                    'TITLE' => $this->input->post('TITLE'),
                    'START_DATE' => $this->input->post('START_DATE'),
                    'END_DATE' => $this->input->post('END_DATE'),
                    'REPEAT_CODE' => $this->input->post('REPEAT_CODE'),
                    'REPEAT_COUNT' => $this->input->post('REPEAT_COUNT'),
                    'LIMIT_COUNT' => $this->input->post('LIMIT_COUNT'),
                    'GIFT_TYPE' => $this->input->post('GIFT_TYPE'),
                    'ITEM_ID' => $this->input->post('ITEM_ID'),
                    'AMOUNT' => $this->input->post('AMOUNT'),
                    'SPOT_START_TIME' => $this->input->post('SPOT_START_TIME'),
                    'SPOT_END_TIME' => $this->input->post('SPOT_END_TIME'),
                );

            $this->Event_data->insertEventRow($data);
        } else if ($this->input->post('oper') == 'edit') {
            $data = array(
                    'EVENT_ID' => $this->input->post('id'),
                    'PROCESS_CODE' => $this->input->post('PROCESS_CODE'),
                    'TITLE' => $this->input->post('TITLE'),
                    'START_DATE' => $this->input->post('START_DATE'),
                    'END_DATE' => $this->input->post('END_DATE'),
                    'REPEAT_CODE' => $this->input->post('REPEAT_CODE'),
                    'REPEAT_COUNT' => $this->input->post('REPEAT_COUNT'),
                    'LIMIT_COUNT' => $this->input->post('LIMIT_COUNT'),
                    'GIFT_TYPE' => $this->input->post('GIFT_TYPE'),
                    'ITEM_ID' => $this->input->post('ITEM_ID'),
                    'AMOUNT' => $this->input->post('AMOUNT'),
                    'SPOT_START_TIME' => $this->input->post('SPOT_START_TIME'),
                    'SPOT_END_TIME' => $this->input->post('SPOT_END_TIME'),
                );

            $this->Event_data->updateEventRow($data);
        } else if ($this->input->post('oper') == 'del') {
            $data = array(
                    'EVENT_ID' => $this->input->post('id')
                );

            $this->Event_data->deleteEventRow($data);
        }
    }
}
