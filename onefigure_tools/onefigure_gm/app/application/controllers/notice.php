<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Notice extends CI_Controller {
    public function __construct() {
        parent::__construct();

        $this->load->model(array('Notice_data'));
        $this->load->helper(array('javascript', 'url', 'form', 'json'));

        $this->load->library('Auth', array('LEVEL' => 1));
    }

    public function index() {
        $this->all();
    }

    public function all($page = 1) {
        $this->load->library('pagination');

        $data = array(
            'PAGE' => $page,
            'LIMIT' => 10,
        );

        $dataArray = $this->Notice_data->selectNotices($data);
        $data['NOTICE_ROWS'] = $dataArray['DATA'];
        $data['NOTICE_TOTAL_COUNT'] = $dataArray['TOTAL_COUNT'];
        $data['NOTICE_TOTAL_PAGE'] = ceil($data['NOTICE_TOTAL_COUNT'] / $data['LIMIT']);

        $this->viewer->all('notice/list', $data);
    }

    public function edit($notice_id = null) {
        $data = array('NOTICE_ID' => null, 'NOTICE_ROW' => null);
        if ($notice_id > 0) {
            $data['NOTICE_ID'] = $notice_id;
            $data['NOTICE_ROW'] = $this->Notice_data->selectNotice($data);
        }

        $this->viewer->all('notice/edit', $data);
    }

    public function ajaxGetNoticeList() {
        $data = array(
            );

        $dataRows = $this->Notice_data->selectNoticeRows($data);

        echo json_encode($dataRows);
    }

    function ajaxGetNotice() {
        $data = array(
                'PAGE' => !$this->input->post('page') ? 1 : $this->input->post('page'),
                'LIMIT' => !$this->input->post('rows') ? 20 : $this->input->post('rows'),
            );

        $dataArray = $this->Notice_data->selectNoticeRows($data);
        $noticeRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        $response = array();
        $response['page'] = $data['PAGE'];
        $response['total'] = ceil($total_count / $data['LIMIT']);
        $response['records'] = $total_count;
        $response['rows'] = $noticeRows;

        json_output($response, true);
    }

    function execSetNotice() {
        if (!$this->input->post('notice_id')) {
            $data = array(
                    'TITLE' => $this->input->post('title'),
                    'CONTENT' => $this->input->post('content'),
                    'MANAGER_ID' => $this->auth->getManagerId(),
                    'START_DATE' => $this->input->post('start_date') . ' 00:00:00',
                    'END_DATE' => $this->input->post('end_date') . ' 23:59:59',
                    'IS_SHOW' => $this->input->post('is_show'),
                );

            $this->Notice_data->insertNotice($data);
        } else {
            $data = array(
                    'NOTICE_ID' => $this->input->post('notice_id'),
                    'TITLE' => $this->input->post('title'),
                    'CONTENT' => $this->input->post('content'),
                    'START_DATE' => $this->input->post('start_date') . ' 00:00:00',
                    'END_DATE' => $this->input->post('end_date') . ' 23:59:59',
                    'IS_SHOW' => $this->input->post('is_show'),
                );

            $this->Notice_data->updateNotice($data);
        }

        js_alert_redirect('Done.', site_url(array('notice')));
    }
}