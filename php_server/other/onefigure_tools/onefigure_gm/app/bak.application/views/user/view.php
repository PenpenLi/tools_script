<script type="text/javascript">
var grid_array = {
    'characters':{
         url:'<?php echo site_url(array($this->uri->segment(1), 'character', 'ajaxGetCharactersOfUserData', $USER_ID)); ?>',
         mtype: "POST",
         postData:{ 'year_month':function() { return $('#search_year_month').val(); } },
         datatype: "json",
         colNames:['Type', 'Slot', 'Level', 'Rune', 'Created'],
         colModel:[
            {name:'CHARACTER_NAME', index:'CHARACTER_NAME', width:150},
            {name:'SLOT_NUMBER', index:'SLOT_NUMBER', width:150},
            {name:'LEVEL', index:'LEVEL', width:150},
            {name:'RUNE_NAME', index:'RUNE_NAME', width:150},
            {name:'CREATE_DATE', index:'CREATE_DATE', width:150}
        ],
        rowNum:20,
        rowList:[20,50,100],
        pager: '#pager',
        viewrecords: true,
        sortorder: "desc",
        height: "100%",
        caption:"Characters"
    },

    'runes':{
         url:'<?php echo site_url(array($this->uri->segment(1), 'rune', 'ajaxGetRunesOfUserData', $USER_ID)); ?>',
         mtype: "POST",
         postData:{ 'year_month':function() { return $('#search_year_month').val(); } },
         datatype: "json",
         colNames:['Name', 'No', 'Level', 'Turn Level', 'Amount', 'Option Count', 'Options', 'Created', 'Deleted'],
         colModel:[
            {name:'RUNE_NAME', index:'item_code', width:200},
            {name:'RUNE_INFO_ID', index:'create_date', width:80},
            {name:'LEVEL', index:'create_date', width:80},
            {name:'TURN_LEVEL', index:'create_date', width:100},
            {name:'AMOUNT', index:'create_date', width:100},
            {name:'OPTION_COUNT', index:'create_date', width:100},
            {name:'OPTIONS', index:'create_date', width:130},
            {name:'CREATE_DATE', index:'create_date', width:130},
            {name:'DELETE_DATE', index:'create_date_google', width:130}
        ],
        rowNum:20,
        rowList:[20,50,100],
        pager: '#pager',
        viewrecords: true,
        sortorder: "desc",
        height: "100%",
        caption:"Runes"
    },

    'payment':{
         url:'<?php echo site_url(array('log', 'pay', 'ajaxGetPaymentOfUserData', $USER_ID)); ?>',
         mtype: "POST",
         postData:{ 'year_month':function() { return $('#search_year_month').val(); } },
         datatype: "json",
         colNames:['Item', 'Created'],
         colModel:[
            {name:'item_code', index:'item_code', width:150},
            {name:'create_date', index:'create_date', width:130}
        ],
        rowNum:20,
        rowList:[20,50,100],
        pager: '#pager',
        viewrecords: true,
        sortorder: "desc",
        width: 600,
        height: "100%",
        caption:"Payment"
    },

    'game_play':{
         url:'<?php echo site_url(array('log', 'game', 'ajaxGetGamePlayOfUserData', $USER_ID)); ?>',
         mtype: "POST",
         postData:{ 'year_month':function() { return $('#search_year_month').val(); } },
         datatype: "json",
         colNames:['Bomb', 'Power', 'Revive', 'Relay', 'Level', 'Score', 'Clear Type', 'Start Date', 'End Date'],
         colModel:[
            {name:'item_code1', index:'item_code1', width:90},
            {name:'item_code2', index:'item_code2', width:90},
            {name:'item_code3', index:'item_code3', width:90},
            {name:'item_code4', index:'item_code4', width:90},
            {name:'level', index:'level', width:50, align:'center', formatter:'integer'},
            {name:'total_score', index:'total_score', width:100, align:'right', formatter:'integer'},
            {name:'clear_type', index:'clear_type', width:100, align:'center'},
            {name:'start_date', index:'start_date', width:130, align:'center'},
            {name:'end_date', index:'end_date', width:130, align:'center'}
        ],
        rowNum:20,
        rowList:[20,50,100],
        pager: '#pager',
        viewrecords: true,
        sortorder: "desc",
        height: "100%",
        caption:"Game Play"
    },

    'send_gift':{
         url:'<?php echo site_url(array('log', 'gift', 'ajaxGetSendGiftOfUserData', $USER_ID)); ?>',
         mtype: "POST",
         postData:{ 'year_month':function() { return $('#search_year_month').val(); } },
         datatype: "json",
         colNames:['번호', '받은 아이디', '보낸 날짜', '받은 날짜'],
         colModel:[
            {name:'id', index:'id', width:80, align:'right'},
            {name:'receive_id', index:'receive_id', width:150},
            {name:'send_date', index:'send_date', width:130},
            {name:'receive_date', index:'receive_date', width:130}
        ],
        rowNum:20,
        rowList:[20,50,100],
        pager: '#pager',
        sortname: 'id',
        viewrecords: true,
        sortorder: "desc",
        width: 600,
        height: "100%",
        caption:"선물 보낸 내역"
    },

    'receive_gift':{
         url:'<?php echo site_url(array('log', 'gift', 'ajaxGetReceiveGiftOfUserData', $USER_ID)); ?>',
         mtype: "POST",
         postData:{ 'year_month':function() { return $('#search_year_month').val(); } },
         datatype: "json",
         colNames:['번호', '보낸 아이디', '보낸 날짜', '받은 날짜'],
         colModel:[
            {name:'id', index:'id', width:80, align:'right'},
            {name:'send_id', index:'send_id', width:150},
            {name:'send_date', index:'send_date', width:130},
            {name:'receive_date', index:'receive_date', width:130}
        ],
        rowNum:20,
        rowList:[20,50,100],
        pager: '#pager',
        viewrecords: true,
        sortorder: "desc",
        width: 600,
        height: "100%",
        caption:"선물 받은 내역"
    },

    'diamond_get':{
         url:'<?php echo site_url(array('log', 'diamond', 'ajaxGetDiamondOfUserData', $USER_ID, 0)); ?>',
         mtype: "POST",
         postData:{ 'year_month':function() { return $('#search_year_month').val(); } },
         datatype: "json",
         colNames:['분류', '다이아몬드', '보유 다이아몬드', '획득 날짜'],
         colModel:[
            {name:'type', index:'type', width:100},
            {name:'diamond', index:'diamond', width:80, formatter:'integer'},
            {name:'current_diamond', index:'current_diamond', width:100, formatter:'integer'},
            {name:'create_date', index:'create_date', width:130}
        ],
        rowNum:20,
        rowList:[20,50,100],
        pager: '#pager',
        viewrecords: true,
        sortorder: "desc",
        width: 600,
        height: "100%",
        caption:"다이아몬드 획득 내역"
    },

    'diamond_use':{
         url:'<?php echo site_url(array('log', 'diamond', 'ajaxGetDiamondOfUserData', $USER_ID, 1)); ?>',
         mtype: "POST",
         postData:{ 'year_month':function() { return $('#search_year_month').val(); } },
         datatype: "json",
         colNames:['아이템', '다이아몬드', '보유 다이아몬드', '사용 날짜'],
         colModel:[
            {name:'item_code', index:'item_code', width:100},
            {name:'diamond', index:'diamond', width:80, formatter:'integer'},
            {name:'current_diamond', index:'current_diamond', width:100, formatter:'integer'},
            {name:'create_date', index:'create_date', width:130}
        ],
        rowNum:20,
        rowList:[20,50,100],
        pager: '#pager',
        viewrecords: true,
        sortorder: "desc",
        width: 600,
        height: "100%",
        caption:"다이아몬드 사용 내역"
    },

    'clover_get':{
         url:'<?php echo site_url(array('log', 'clover', 'ajaxGetCloverOfUserData', $USER_ID, 0)); ?>',
         mtype: "POST",
         postData:{ 'year_month':function() { return $('#search_year_month').val(); } },
         datatype: "json",
         colNames:['분류', '클로버', '보유 클로버', '획득 날짜'],
         colModel:[
            {name:'type', index:'type', width:100},
            {name:'clover', index:'clover', width:80, formatter:'integer'},
            {name:'current_clover', index:'current_clover', width:100, formatter:'integer'},
            {name:'create_date', index:'create_date', width:130}
        ],
        rowNum:20,
        rowList:[20,50,100],
        pager: '#pager',
        viewrecords: true,
        sortorder: "desc",
        width: 600,
        height: "100%",
        caption:"클로버 획득 내역"
    },

    'clover_use':{
         url:'<?php echo site_url(array('log', 'clover', 'ajaxGetCloverOfUserData', $USER_ID, 1)); ?>',
         mtype: "POST",
         postData:{ 'year_month':function() { return $('#search_year_month').val(); } },
         datatype: "json",
         colNames:['아이템', '클로버', '보유 클로버', '사용 날짜'],
         colModel:[
            {name:'item_code', index:'item_code', width:100},
            {name:'clover', index:'clover', width:80, formatter:'integer'},
            {name:'current_clover', index:'current_clover', width:100, formatter:'integer'},
            {name:'create_date', index:'create_date', width:130}
        ],
        rowNum:20,
        rowList:[20,50,100],
        pager: '#pager',
        viewrecords: true,
        sortorder: "desc",
        width: 600,
        height: "100%",
        caption:"클로버 사용 내역"
    },

    'banner':{
         url:'<?php echo site_url(array('log', 'banner', 'ajaxGetBannerOfUserData', $USER_ID)); ?>',
         mtype: "POST",
         postData:{ 'year_month':function() { return $('#search_year_month').val(); } },
         datatype: "json",
         colNames:['게임명', '상태', '클릭 날짜', '보상 지급 날짜'],
         colModel:[
            {name:'game_name', index:'game_name', width:100},
            {name:'state', index:'state', width:80},
            {name:'click_date', index:'click_date', width:130},
            {name:'pay_date', index:'pay_date', width:130}
        ],
        rowNum:20,
        rowList:[20,50,100],
        pager: '#pager',
        viewrecords: true,
        sortorder: "desc",
        width: 600,
        height: "100%",
        caption:"배너 참여 내역"
    }
};

$(function() {
    $('#search_year_month').change(function() {
        viewGrid();
    });

    $(".reason-form1").validate({
        "submitHandler":function(form) {
            $("#reason-form-modal").modal("show");
        }
    });
});

var GRID_TYPE = '';
function viewGrid(grid_type) {
    var gt = grid_type || GRID_TYPE;
    GRID_TYPE = gt;

    $('#grid').empty().append('<table id="log_table"></table>').append('<div id="pager"></div>');
    $('#log_table').jqGrid(grid_array[gt]);
    $('#pager').jqGrid('navGrid','#pager',{edit:false,add:false,del:false});
}
</script>

<div class="row small">
    <div class="col-md-4">
        <div class="panel panel-default">
            <div class="panel-heading">
                <?php if ($USER_ROW['USER_ID'] < 0) { ?><span class="label label-danger">Dummy</span><?php } ?>
                <?php if ($USER_ROW['STATUS'] == 0) { ?><span class="label label-warning">Invalid</span><?php } ?>
                <strong><?php echo $USER_ROW['NICKNAME']; ?></strong>
            </div>

            <table class="table table-bordered">
                <colgroup>
                    <col width="120" />
                    <col />
                </colgroup>

                <tbody>
                <tr>
                    <th>User ID</th>
                    <td><?php echo $USER_ROW['USER_ID']; ?></td>
                </tr>
                <tr>
                    <th>Platform ID</th>
                    <td><?php echo $USER_ROW['EMAIL']; ?></td>
                </tr>
                <tr>
                    <th>Nickname</th>
                    <td><?php echo $USER_ROW['NICKNAME']; ?></td>
                </tr>
                <tr>
                    <th>Level</th>
                    <td><?php echo $USER_ROW['LEVEL']; ?></td>
                </tr>
                <tr>
                    <th>Mystique Slots</th>
                    <td><?php echo $USER_ROW['SKILL_SLOT']; ?> / <?php echo MAX_SKILL_SLOT; ?></td>
                </tr>
                <tr>
                    <th>Artifact Slots</th>
                    <td><?php echo $USER_ROW['TREASURE_SLOT']; ?> / <?php echo MAX_TREASURE_SLOT; ?></td>
                </tr>
                <tr>
                    <th>Artifact Inventory</th>
                    <td><?php echo $USER_ROW['TREASURE_INVENTORY']; ?> / <?php echo MAX_TREASURE_INVENTORY; ?></td>
                </tr>
                <tr>
                    <th>Device</th>
                    <td><?php echo $USER_ROW['DEVICE']; ?></td>
                </tr>
                <tr>
                    <th>OS</th>
                    <td><?php echo $USER_ROW['OS_VERSION']; ?></td>
                </tr>
                <tr>
                    <th>Created</th>
                    <td><?php echo $USER_ROW['CREATE_DATE']; ?> (<?php echo $USER_ROW['CREATE_IP']; ?>)</td>
                </tr>
                <tr>
                    <th>Last Login</th>
                    <td><?php echo $USER_ROW['LOGIN_DATE']; ?> (<?php echo $USER_ROW['LOGIN_IP']; ?>)</td>
                </tr>
                </tbody>
            </table>
        </div>
    </div>
    <div class="col-md-8">
        <div role="panel">
            <ul class="nav nav-tabs nav-justified" role="tablist">
                <li role="presentation" class="active"><a href="#point" aria-controls="point" role="tab" data-toggle="tab">Point</a></li>
                <li role="presentation"><a href="#items" aria-controls="items" role="tab" data-toggle="tab">Items</a></li>
                <li role="presentation"><a href="#stance" aria-controls="stance" role="tab" data-toggle="tab">Stance <span class="badge"><?php echo number_format(count($CHARACTER_ROWS)); ?></span></a></li>
                <li role="presentation"><a href="#mystique" aria-controls="mystique" role="tab" data-toggle="tab">Mystique <span class="badge"><?php echo number_format(count($SKILL_ROWS)); ?></span></a></li>
                <li role="presentation"><a href="#artifact" aria-controls="artifact" role="tab" data-toggle="tab">Artifact <span class="badge"><?php echo number_format(count($TREASURE_ROWS)); ?></span></a></li>
                <li role="presentation"><a href="#stage" aria-controls="stage" role="tab" data-toggle="tab">Stage <span class="badge"><?php echo number_format(count($STAGE_ROWS)); ?></span></a></li>
                <li role="presentation"><a href="#achievement" aria-controls="achievement" role="tab" data-toggle="tab">Achievement <span class="badge"><?php echo number_format(count($ACHIEVEMENT_ROWS)); ?></span></a></li>
            </ul>

            <div class="tab-content">
                <div class="tab-pane in active" id="point">
                    <table class="table table-hover table-bordered">
                        <col width="140" />
                        <col width="140" />
                        <col />
                        <col width="80" />

                        <thead>
                        <tr>
                            <th class="text-center">Type</th>
                            <th class="text-center">Amount</th>
                            <th class="text-center">Add (+-)</th>
                            <th class="text-center">Action</th>
                        </tr>
                        </thead>

                        <tbody>
                        <?php echo form_open(site_url(array('user', 'execAddPoint', $USER_ID)), array('method' => 'post', 'name' => 'add-money-form', 'class' => 'reason-form')); ?>
                        <input type="hidden" name="point_type" value="MONEY" />
                        <tr>
                            <td class="text-center">Money</td>
                            <td class="text-center"><?php echo number_format($USER_ROW['MONEY']); ?></td>
                            <td class="text-center">
                                <input type="number" name="amount" class="form-control input-sm" required="required" />
                            </td>
                            <td class="text-center">
                                <button type="submit" class="btn btn-primary btn-sm">Save</button>
                            </td>
                        </tr>
                        <?php echo form_close(); ?>
                        <?php echo form_open(site_url(array('user', 'execAddPoint', $USER_ID)), array('method' => 'post', 'name' => 'add-cash-form', 'class' => 'reason-form')); ?>
                        <input type="hidden" name="point_type" value="CASH" />
                        <tr>
                            <td class="text-center">Cash</td>
                            <td class="text-center"><?php echo number_format($USER_ROW['CASH']); ?></td>
                            <td class="text-center">
                                <input type="number" name="amount" class="form-control input-sm" />
                            </td>
                            <td class="text-center">
                                <button type="submit" class="btn btn-primary btn-sm">Save</button>
                            </td>
                        </tr>
                        <?php echo form_close(); ?>
                        <!--
                        <?php echo form_open(site_url(array('user', 'execAddPoint', $USER_ID)), array('method' => 'post', 'name' => 'add-heart-form', 'class' => 'reason-form')); ?>
                        <input type="hidden" name="point_type" value="HEART" />
                        <tr>
                            <td class="text-center">Heart</td>
                            <td class="text-center"><?php echo number_format($USER_ROW['HEART']); ?></td>
                            <td class="text-center">
                                <input type="number" name="amount" class="form-control input-sm" />
                            </td>
                            <td class="text-center">
                                <button type="submit" class="btn btn-primary btn-sm">Save</button>
                            </td>
                        </tr>
                        <?php echo form_close(); ?>
                        -->
                        <?php echo form_open(site_url(array('user', 'execAddPoint', $USER_ID)), array('method' => 'post', 'name' => 'add-lottery-point-form', 'class' => 'reason-form')); ?>
                        <input type="hidden" name="point_type" value="LOTTERY_POINT" />
                        <tr>
                            <td class="text-center">Present Box</td>
                            <td class="text-center"><?php echo number_format($USER_ROW['LOTTERY_POINT']); ?></td>
                            <td class="text-center">
                                <input type="number" name="amount" class="form-control input-sm" />
                            </td>
                            <td class="text-center">
                                <button type="submit" class="btn btn-primary btn-sm">Save</button>
                            </td>
                        </tr>
                        <?php echo form_close(); ?>
                        <?php echo form_open(site_url(array('user', 'execAddPoint', $USER_ID)), array('method' => 'post', 'name' => 'add-lottery-high-coupon-form', 'class' => 'reason-form')); ?>
                        <input type="hidden" name="point_type" value="LOTTERY_HIGH_COUPON" />
                        <tr>
                            <td class="text-center">High Ticket</td>
                            <td class="text-center"><?php echo number_format($USER_ROW['LOTTERY_HIGH_COUPON']); ?></td>
                            <td class="text-center">
                                <input type="number" name="amount" class="form-control input-sm" />
                            </td>
                            <td class="text-center">
                                <button type="submit" class="btn btn-primary btn-sm">Save</button>
                            </td>
                        </tr>
                        <?php echo form_close(); ?>
                        <?php echo form_open(site_url(array('user', 'execAddPoint', $USER_ID)), array('method' => 'post', 'name' => 'add-lottery-coupon-form', 'class' => 'reason-form')); ?>
                        <input type="hidden" name="point_type" value="LOTTERY_COUPON" />
                        <tr>
                            <td class="text-center">Ticket</td>
                            <td class="text-center"><?php echo number_format($USER_ROW['LOTTERY_COUPON']); ?></td>
                            <td class="text-center">
                                <input type="number" name="amount" class="form-control input-sm" />
                            </td>
                            <td class="text-center">
                                <button type="submit" class="btn btn-primary btn-sm">Save</button>
                            </td>
                        </tr>
                        <?php echo form_close(); ?>
                        </tbody>
                    </table>
                </div>
                <div class="tab-pane in" id="items">
                    <table class="table table-bordered table-striped">
                        <colgroup>
                            <col />
                            <col width="80" />
                            <col width="150" />
                        </colgroup>

                        <thead>
                        <tr>
                            <th class="text-center">Name</th>
                            <th class="text-center">Amount</th>
                            <th class="text-center">Action</th>
                        </tr>
                        </thead>

                        <tbody>
                        <?php foreach ($INSTANT_ITEM_ROWS as $row) { ?>
                            <tr>
                                <td><?php echo $row['NAME']; ?> (<?php echo $row['INSTANT_ITEM_ID']; ?>)</td>
                                <td class="text-center"><?php echo $row['AMOUNT']; ?></td>
                                <td class="text-center">
                                    <button type="button" class="btn btn-xs btn-primary">Lv ▲</button>
                                    <button type="button" class="btn btn-xs btn-warning">Lv ▼</button>
                                    <button type="button" class="btn btn-xs btn-danger">Delete</button>
                                </td>
                            </tr>
                        <?php } ?>
                        </tbody>
                    </table>
                </div>
                <div class="tab-pane in" id="stance">
                    <table class="table table-bordered table-striped">
                        <colgroup>
                            <col width="100" />
                            <col />
                            <col width="80" />
                            <col width="150" />
                            <col width="150" />
                        </colgroup>

                        <thead>
                        <tr>
                            <th class="text-center">ID</th>
                            <th class="text-center">Name</th>
                            <th class="text-center">Level</th>
                            <th class="text-center">Created</th>
                            <th class="text-center">Action</th>
                        </tr>
                        </thead>

                        <tbody>
                        <?php foreach ($CHARACTER_ROWS as $row) { ?>
                        <tr>
                            <td class="text-center"><?php echo $row['CHARACTER_ID']; ?></td>
                            <td><?php echo $row['NAME']; ?> (<?php echo $row['CHARACTER_INFO_ID']; ?>)</td>
                            <td class="text-center"><?php echo $row['LEVEL']; ?></td>
                            <td class="text-center"><?php echo $row['CREATE_DATE']; ?></td>
                            <td class="text-center">
                                <button type="button" class="btn btn-xs btn-primary" <?php echo $row['LEVEL'] == MAX_CHARACTER_LEVEL ? 'disabled="disabled"' : ''; ?>>Lv ▲</button>
                                <button type="button" class="btn btn-xs btn-warning" <?php echo $row['LEVEL'] == 1 ? 'disabled="disabled"' : ''; ?>>Lv ▼</button>
                                <button type="button" class="btn btn-xs btn-danger" <?php echo $row['CHARACTER_INFO_ID'] == 1 ? 'disabled="disabled"' : ''; ?>>Delete</button>
                            </td>
                        </tr>
                        <?php } ?>
                        </tbody>
                    </table>
                </div>
                <div class="tab-pane in" id="mystique">
                    <table class="table table-bordered table-striped">
                        <colgroup>
                            <col width="100" />
                            <col />
                            <col width="80" />
                            <col width="150" />
                            <col width="150" />
                        </colgroup>

                        <thead>
                        <tr>
                            <th class="text-center">ID</th>
                            <th class="text-center">Name</th>
                            <th class="text-center">Level</th>
                            <th class="text-center">Created</th>
                            <th class="text-center">Action</th>
                        </tr>
                        </thead>

                        <tbody>
                        <?php foreach ($SKILL_ROWS as $row) { ?>
                            <tr>
                                <td class="text-center"><?php echo $row['SKILL_ID']; ?></td>
                                <td><?php echo $row['NAME']; ?> (<?php echo $row['SKILL_INFO_ID']; ?>)</td>
                                <td class="text-center"><?php echo $row['LEVEL']; ?></td>
                                <td class="text-center"><?php echo $row['CREATE_DATE']; ?></td>
                                <td class="text-center">
                                    <button type="button" class="btn btn-xs btn-primary" <?php echo $row['LEVEL'] == MAX_SKILL_LEVEL ? 'disabled="disabled"' : ''; ?>>Lv ▲</button>
                                    <button type="button" class="btn btn-xs btn-warning" <?php echo $row['LEVEL'] == 1 ? 'disabled="disabled"' : ''; ?>>Lv ▼</button>
                                    <button type="button" class="btn btn-xs btn-danger">Delete</button>
                                </td>
                            </tr>
                        <?php } ?>
                        </tbody>
                    </table>
                </div>
                <div class="tab-pane in" id="artifact">
                    <table class="table table-bordered table-striped">
                        <colgroup>
                            <col width="100" />
                            <col />
                            <col width="80" />
                            <col width="150" />
                            <col width="150" />
                        </colgroup>

                        <thead>
                        <tr>
                            <th class="text-center">ID</th>
                            <th class="text-center">Name</th>
                            <th class="text-center">Level</th>
                            <th class="text-center">Created</th>
                            <th class="text-center">Action</th>
                        </tr>
                        </thead>

                        <tbody>
                        <?php foreach ($TREASURE_ROWS as $row) { ?>
                            <tr>
                                <td class="text-center"><?php echo $row['TREASURE_ID']; ?></td>
                                <td><?php echo $row['NAME']; ?> (<?php echo $row['TREASURE_INFO_ID']; ?>)</td>
                                <td class="text-center"><?php echo $row['LEVEL']; ?></td>
                                <td class="text-center"><?php echo $row['CREATE_DATE']; ?></td>
                                <td class="text-center">
                                    <button type="button" class="btn btn-xs btn-primary" <?php echo $row['LEVEL'] == MAX_TREASURE_LEVEL ? 'disabled="disabled"' : ''; ?>>Lv ▲</button>
                                    <button type="button" class="btn btn-xs btn-warning" <?php echo $row['LEVEL'] == 1 ? 'disabled="disabled"' : ''; ?>>Lv ▼</button>
                                    <button type="button" class="btn btn-xs btn-danger">Delete</button>
                                </td>
                            </tr>
                        <?php } ?>
                        </tbody>
                    </table>
                </div>
                <div class="tab-pane in" id="stage">
                    <table class="table table-bordered table-striped">
                        <colgroup>
                            <col />
                            <col width="80" />
                            <col width="100" />
                            <col width="100" />
                            <col width="150" />
                            <col width="80" />
                        </colgroup>

                        <thead>
                        <tr>
                            <th class="text-center">Name</th>
                            <th class="text-center">Clear Type</th>
                            <th class="text-center">Cleared Count</th>
                            <th class="text-center">Best Score</th>
                            <th class="text-center">Created</th>
                            <th class="text-center">Action</th>
                        </tr>
                        </thead>

                        <tbody>
                        <?php foreach ($STAGE_ROWS as $index => $row) { ?>
                            <tr>
                                <td><?php echo $row['NAME']; ?></td>
                                <td class="text-center"><?php echo $row['CLEAR_TYPE']; ?></td>
                                <td class="text-center"><?php echo number_format($row['CLEAR_COUNT']); ?></td>
                                <td class="text-center"><?php echo number_format($row['BEST_SCORE']); ?></td>
                                <td class="text-center"><?php echo $row['CREATE_DATE']; ?></td>
                                <td class="text-center">
                                    <button type="button" class="btn btn-xs btn-danger" <?php echo ($index + 1) < count($STAGE_ROWS) ? 'disabled="disabled"' : ''; ?>>Delete</button>
                                </td>
                            </tr>
                        <?php } ?>
                        </tbody>
                    </table>
                </div>
                <div class="tab-pane in" id="achievement">
                    <table class="table table-bordered table-striped">
                        <colgroup>
                            <col width="60" />
                            <col />
                            <col width="150" />
                            <col width="80" />
                        </colgroup>

                        <thead>
                        <tr>
                            <th class="text-center">ID</th>
                            <th class="text-center">Name</th>
                            <th class="text-center">Progress</th>
                            <th class="text-center">Reward</th>
                        </tr>
                        </thead>

                        <tbody>
                        <?php foreach ($ACHIEVEMENT_ROWS as $row) { ?>
                            <tr>
                                <td class="text-center"><?php echo $row['ACHIEVEMENT_INFO_ID']; ?></td>
                                <td><?php echo $row['NAME']; ?></td>
                                <td class="text-center"><?php echo number_format($row['PROGRESS']); ?> / <?php echo number_format($row['MAX_COUNT']); ?></td>
                                <td class="text-center"><?php echo $row['REWARD_OK'] == 1 ? '<span class="label label-success">OK</span>' : '<span class="label label-default">Not yet</span>'; ?></td>
                            </tr>
                        <?php } ?>
                        </tbody>
                    </table>
                </div>
            </div>

        </div>
    </div>
</div>

<div class="modal fade" id="reason-form-modal" tabindex="-1" role="dialog" aria-labelledby="modalReasonLabel" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal"><span aria-hidden="true">&times;</span><span class="sr-only">Close</span></button>
                <h4 class="modal-title" id="modalReasonLabel">Reason (optional)</h4>
            </div>
            <div class="modal-body">
                <textarea class="form-control" rows="5"></textarea>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
                <button type="button" class="btn btn-primary">Save changes</button>
            </div>
        </div>
    </div>
</div>

<!--
<div class="row small">
  <div class="col-md-2">
    <select id="search_year_month" class="form-control">
    <?php
    $start_month_date = '2014-07-01';
    $yesterday_month_date = date('Y-m-01', strtotime('-1 day'));
    $loop = 0;

    while ($current_month_date < $yesterday_month_date) {
        $current_month_time = strtotime($start_month_date . ' +' . $loop . ' month');
        $current_month_date = date('Y-m-01', $current_month_time);
    ?>
      <option value="<?php echo date('Ym', $current_month_time); ?>" <?php echo (date('Y-m-01', $current_month_time) == $yesterday_month_date) ? ' selected="selected" ' : ''; ?>><?php echo date('Y-m', $current_month_time); ?></option>
    <?php
        $loop++;
    }
    ?>
    </select>
  </div>
  <div class="col-md-8">
    <ul class="nav nav-pills">
      <li><a href="javascript:;" onclick="viewGrid('characters');">Characters</a></li>
      <li><a href="javascript:;" onclick="viewGrid('runes');">Runes</a></li>
      <li><a href="javascript:;" onclick="viewGrid('payment');">Payments</a></li>
      <li><a href="javascript:;" onclick="viewGrid('game_play');">Game Play</a></li>
    </ul>
  </div>
</div>

<div id="grid"></div>
-->