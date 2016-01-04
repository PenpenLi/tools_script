<?php
if (! defined('BASEPATH')) exit('No direct script access allowed');

class Manager extends CI_Controller
{
	public $keysmap = array(
		"bbr_Dialog_StarterPackEx" => "bbr弹窗_新手礼包",
		"bbr_Dialog_Buy_MonsterTruck_Pack" => "bbr弹窗_蓝翔礼包",
		"bbr_Dialog_Buy_MonsterTruck_Pack_Discount" => "bbr弹窗_蓝翔打折礼包",	
		"bbr_Dialog_Tuhao_Pack" => "bbr弹窗_土豪礼包",
		"bbr_Dialog_Tuhao_PackByPC" => "bbr弹窗_土豪礼包",
		"bbr_Dialog_CarMoon" => "bbr弹窗_月球漫步者",
		"bbr_Dialog_SuperVip_Pack" => "bbr弹窗_超级vip",
		"bbr_Button_StarterPackEx" => "bbr按钮_新手礼包",
		"bbr_Button_Dialog_Tuhao_Pack" => "bbr按钮_土豪礼包", 
		"bbr_Button_Dialog_CarMoon" => "bbr按钮_月球漫步者",
		"bbr_Button_Dialog_SuperVip_Pack" => "bbr按钮_超级vip",
		"0" => "弹窗关闭",
		"1" => "弹窗打开",
		"2" => "礼包领取",
		"3" => "礼包购买"
	);
	
	public function __construct() {
		parent::__construct();
		$this->load->model("Manager_model");
		$this->load->model("GameInfo_model");
		$this->load->library('session');
	}

	public function index() {
		$this->load->view('login_view');
	}

	public function login() {
		$this->index();
	}

	public function plusVersion() {
	    $configFilePath = '/home/www/itcommon/php/ip2info_gm/app/config_version.json';
	    $config = file_get_contents($configFilePath);
	    $config = json_decode($config, TRUE);
	    $config['version'] = $config['version'] + 1;
	    file_put_contents($configFilePath, json_encode($config));
	}	
	
	public function gameinfo() {
		$data = array(
            'USERNAME' => $this->input->post('username'),
            'PASSWORD' => $this->input->post('pw')
        );

        if (!$data['USERNAME']) {
        	echo '用户名为空';
        	return;
        }
        if (!$data['PASSWORD']) {
        	echo '密码为空';
        	return;
        }
        $managerInfo = $this->Manager_model->selectManagerInfo($data);
        if ($managerInfo['PASSWORD'] != $data['PASSWORD']) {
        	echo '用户名或者密码错误!';
        	return;
        }

        $this->session->set_userdata('USERNAME', 'USERNAME');
        $this->session->set_userdata('PASSWORD', 'PASSWORD');

        $viewData = array();
        $gameinfo = $this->GameInfo_model->selectGameInfo(array());
        $viewData['items'] = $gameinfo;
        $viewData['keysmap'] = $this->keysmap;
        $this->load->view('gameinfo_view', $viewData);
	}

	public function query() {
		if (!$this->session->has_userdata('USERNAME')) {
			return;
		}
		$data = array();
		if ($this->input->post('game_name') != "no_limit") {
			$data['GAME_NAME'] = $this->input->post('game_name');
		}
		if ($this->input->post('platform') != "no_limit") {
			$data['PLATFORM'] = $this->input->post('platform');
		}
		if ($this->input->post('province') != "no_limit") {
			$data['PROVINCE'] = $this->input->post('province');
		}
		if ($this->input->post('wireless_type') != "no_limit") {
			$data['WIRELESS_TYPE'] = $this->input->post('wireless_type');
		}
		if ($this->input->post('item_key') != "no_limit") {
			$data['ITEM_KEY'] = $this->input->post('item_key');
		}
        $gameinfo = $this->GameInfo_model->selectGameInfo($data);
        $viewData['items'] = $gameinfo;
        $viewData['keysmap'] = $this->keysmap;
        $this->load->view('gameinfo_view', $viewData);
	}

	public function add() {
		if (!$this->session->has_userdata('USERNAME')) {
			return;
		}
		$data = array();
		if ($this->input->post('game_name') != "no_limit") {
			$data['GAME_NAME'] = $this->input->post('game_name');
		}
		if ($this->input->post('platform') != "no_limit") {
			$data['PLATFORM'] = $this->input->post('platform');
		}
		if ($this->input->post('province') != "no_limit") {
			$data['PROVINCE'] = $this->input->post('province');
		}
		if ($this->input->post('wireless_type') != "no_limit") {
			$data['WIRELESS_TYPE'] = $this->input->post('wireless_type');
		}
		if ($this->input->post('item_key') != "no_limit") {
			$data['ITEM_KEY'] = $this->input->post('item_key');
		}
		if ($this->input->post('item_value') != "no_limit") {
			$data['ITEM_VALUE'] = $this->input->post('item_value');
		}
		$isConfigValid = true;
		if ($this->input->post('item_key') == "bbr_Dialog_StarterPackEx" ||
			$this->input->post('item_key') == "bbr_Dialog_Buy_MonsterTruck_Pack" ||
			$this->input->post('item_key') == "bbr_Dialog_Buy_MonsterTruck_Pack_Discount" ||
			$this->input->post('item_key') == "bbr_Dialog_Tuhao_PackByPC" ||
			$this->input->post('item_key') == "bbr_Dialog_CarMoon" ||
			$this->input->post('item_key') == "bbr_Dialog_SuperVip_Pack") {
			if ($this->input->post('item_value') != "0" && $this->input->post('item_value') != "1") {
				echo "配置错误1，不允许这样的配置:" . $this->input->post('item_key') . "," . $this->input->post('item_value');
				$isConfigValid = false;
			}
		} else {
			if ($this->input->post('item_value') != "2" && $this->input->post('item_value') != "3") {
				//echo "配置错误，不允许这样的配置";
				echo "配置错误1，不允许这样的配置:" . $this->input->post('item_key') . "," . $this->input->post('item_value');
				$isConfigValid = false;
			}
		}

		if ($this->GameInfo_model->selectGameInfo($data)) {
			echo "配置已经存在";
			$isConfigValid = false;
		}
		
		if ($isConfigValid) {
			$this->GameInfo_model->insertGameInfo($data);
		}
		$gameinfo = $this->GameInfo_model->selectGameInfo(array());

	        $viewData['items'] = $gameinfo;
       		$viewData['keysmap'] = $this->keysmap;
      		$this->load->view('gameinfo_view', $viewData);

        	$this->plusVersion();
	}

	public function modify() {
		if (!$this->session->has_userdata('USERNAME')) {
			return;
		}		
		$data = array();
		if ($this->input->post('game_name') != "no_limit") {
			$data['GAME_NAME'] = $this->input->post('game_name');
		}
		if ($this->input->post('platform') != "no_limit") {
			$data['PLATFORM'] = $this->input->post('platform');
		}
		if ($this->input->post('province') != "no_limit") {
			$data['PROVINCE'] = $this->input->post('province');
		}
		if ($this->input->post('wireless_type') != "no_limit") {
			$data['WIRELESS_TYPE'] = $this->input->post('wireless_type');
		}
		if ($this->input->post('item_key') != "no_limit") {
			$data['ITEM_KEY'] = $this->input->post('item_key');
		}
		if ($this->input->post('item_value') != "no_limit") {
			$data['ITEM_VALUE'] = $this->input->post('item_value');
		}

		$this->GameInfo_model->modifyGameInfo($data);
		$gameinfo = $this->GameInfo_model->selectGameInfo(array());
        $viewData['items'] = $gameinfo;
        $viewData['keysmap'] = $this->keysmap;
        $this->load->view('gameinfo_view', $viewData);

        $this->plusVersion();
	}

	public function remove() {
		if (!$this->session->has_userdata('USERNAME')) {
			return;
		}
		$data = array();
		if ($this->input->post('game_name') != "no_limit") {
			$data['GAME_NAME'] = $this->input->post('game_name');
		}
		if ($this->input->post('platform') != "no_limit") {
			$data['PLATFORM'] = $this->input->post('platform');
		}
		if ($this->input->post('province') != "no_limit") {
			$data['PROVINCE'] = $this->input->post('province');
		}
		if ($this->input->post('wireless_type') != "no_limit") {
			$data['WIRELESS_TYPE'] = $this->input->post('wireless_type');
		}
		if ($this->input->post('item_key') != "no_limit") {
			$data['ITEM_KEY'] = $this->input->post('item_key');
		}

		$this->GameInfo_model->deleteGameInfo($data);
		$gameinfo = $this->GameInfo_model->selectGameInfo(array());
        $viewData['items'] = $gameinfo;
        $viewData['keysmap'] = $this->keysmap;
        $this->load->view('gameinfo_view', $viewData);

        $this->plusVersion();
	}
}

?>

