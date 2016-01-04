<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Cron extends CI_Controller {
    public function __construct() {
        parent::__construct();

        $this->load->model(array(
            'Cron_data',
        ));
    }

    public function user($start = null, $end = null) {
        if (!$start && !$end) {
            $start = date('Y-m-d', strtotime('-1 day'));
            $end = date('Y-m-d', strtotime('-1 day'));
        }

        $data = array(
            'START_DATE' => $start,
            'END_DATE' => $end,
        );

        $this->Cron_data->userCount(array('START_DATE' => date('Y-m-d', strtotime('-1 day'))));
        $this->Cron_data->userJoinCount($data);
        $this->Cron_data->userLoginCount($data);

        echo "User data update!\n";
    }

    public function payment($start = null, $end = null) {
        if (!$start && !$end) {
            $start = date('Y-m-d', strtotime('-1 day'));
            $end = date('Y-m-d', strtotime('-1 day'));
        }

        $data = array(
            'START_DATE' => $start,
            'END_DATE' => $end,
        );

        $this->Cron_data->paymentCount($data);
        $this->Cron_data->paymentUserCount($data);

        echo "Payment data update!\n";
    }

    public function point($start = null, $end = null) {
        if (!$start && !$end) {
            $start = date('Y-m-d', strtotime('-1 day'));
            $end = date('Y-m-d', strtotime('-1 day'));
        }

        $data = array(
            'START_DATE' => $start,
            'END_DATE' => $end,
        );

        $this->Cron_data->pointCount($data);

        echo "Point data update!\n";
    }

    public function game($start = null, $end = null) {
        if (!$start && !$end) {
            $start = date('Y-m-d', strtotime('-1 day'));
            $end = date('Y-m-d', strtotime('-1 day'));
        }

        $data = array(
            'START_DATE' => $start,
            'END_DATE' => $end,
        );

        $this->Cron_data->gameStage(array('START_DATE' => date('Y-m-d', strtotime('-1 day'))));
        $this->Cron_data->gameCharacter(array('START_DATE' => date('Y-m-d', strtotime('-1 day'))));
        $this->Cron_data->gameSkill(array('START_DATE' => date('Y-m-d', strtotime('-1 day'))));
        $this->Cron_data->gameTreasure(array('START_DATE' => date('Y-m-d', strtotime('-1 day'))));
        $this->Cron_data->gamePlayCount($data);

        echo "Game data update!\n";
    }
}