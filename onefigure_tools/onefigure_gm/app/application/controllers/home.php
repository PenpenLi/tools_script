<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Home extends CI_Controller {
    function __construct() {
        parent::__construct();

        $this->load->model(array('Realtime_data'));
        $this->load->helper(array('javascript', 'url', 'form'));

        $this->load->library('Auth', array('LEVEL' => 1));
    }

    function index() {
        $user_total_count = $this->Realtime_data->getUserTotalCount();
        $user_daily_joined = $this->Realtime_data->getUserDailyJoinCountRow();
        $user_daily_payment = $this->Realtime_data->getUserDailyPaymentCountRow();
        $user_daily_payment_IOS = $this->Realtime_data->getUserDailyPaymentCountRow_IOS();
        $user_daily_payment_Naver = $this->Realtime_data->getUserDailyPaymentCountRow_Naver();
        $data = array(
                'USER_TOTAL_COUNT' => $user_total_count,
                'USER_DAILY_JOINED' => $user_daily_joined,
                'USER_DAILY_PAYMENT' => $user_daily_payment,
                'USER_DAILY_PAYMENT_IOS' => $user_daily_payment_IOS,
                'USER_DAILY_PAYMENT_Naver' => $user_daily_payment_Naver,
            );

        $this->viewer->all('dash_board', $data);
    }
}
