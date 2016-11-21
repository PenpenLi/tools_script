<script type="text/javascript">
google.load("visualization", "1", {packages:["corechart"]});

var OS_NAME_ARRAY = <?php echo json_encode($OS_NAME_ARRAY); ?>;

function drawPieChart(title, data_array) {
    var data = google.visualization.arrayToDataTable(data_array);

    var options = {
        'title':title,
        'width':950,
        'height':550,
        'fontSize':11,
        'chartArea':{left:60,top:20,width:800,height:500}
    };

    var chart = new google.visualization.PieChart(document.getElementById('chart_div'));
    chart.draw(data, options);
}

function clickChartButton(title, grid_id) {
    var selected_rows_id = $(grid_id).jqGrid('getGridParam', 'selarrrow');
    var selected_rows = null;

    selected_rows_id.sort();

    if (selected_rows_id.length == 0) {
        alert('데이터를 선택해 주세요.');
        return false;
    }

    var data_array = [['아이템 조합', '플레이수']];

    for (var index in selected_rows_id) {
        selected_rows = $(grid_id).jqGrid('getRowData', selected_rows_id[index]);
        data_array[parseInt(index) + 1] = [selected_rows['ITEMS_NAME'], parseInt(selected_rows['TOTAL_COUNT'])];
    }

    drawPieChart(title, data_array);
    $('#chart-viewer').dialog('open');
}

function showUserJoinedCountOfOS() {
    $('#grid-area').empty();

    var columns_name = ['Date', 'Sum'];
    var columns_detail = [
            {name:'STAT_DATE', index:'STAT_DATE', width:90, align:"center", sortable:false, frozen:true},
            {name:'TOTAL_COUNT', index:'TOTAL_COUNT', width:70, align:"right", sortable:false, formatter:'integer', frozen:true}
        ];

    for (var i = columns_name.length, index = 1; index < OS_NAME_ARRAY.length; i++, index++) {
        columns_name[i] = OS_NAME_ARRAY[index];
        columns_detail[i] = {name:'USER_COUNT_' + index, index:'USER_COUNT_' + index, width:70, align:"right", sortable:false, formatter:'integer'};
    }

    $('<table id="count_table"></table>').appendTo($('#grid-area')).jqGrid({
        url:'<?php echo site_url(array('log', 'os', 'ajaxGetUserJoinedCountOfOSData')); ?>',
        mtype: "POST",
        postData:{ 'start_date':function() { return $('#start_date').val(); }, 'end_date':function() { return $('#end_date').val(); } },
        datatype: "json",
        colNames:columns_name,
        colModel:columns_detail,
        rowNum:-1,
        multiselect:true,
        shrinkToFit:false,
        viewrecords:true,
        sortorder:"desc",
        jsonReader:{
            repeatitems:false,
            root:"rows"
        },
        width:1000,
        height:"auto",
        //toolbar:[true,"top"],
        caption:"User joined count"
    });

    $("#count_table").jqGrid('setFrozenColumns');
    $("input","#t_count_table").click(function() { clickChartButton('User Count by Level', '#count_table'); });
}

function showActiveUserCountOfOS() {
    $('#grid-area').empty();

    var columns_name = ['Date', 'Sum'];
    var columns_detail = [
            {name:'STAT_DATE', index:'STAT_DATE', width:90, align:"center", sortable:false, frozen:true},
            {name:'TOTAL_COUNT', index:'TOTAL_COUNT', width:70, align:"right", sortable:false, formatter:'integer', frozen:true}
        ];

    for (var i = columns_name.length, index = 1; index < OS_NAME_ARRAY.length; i++, index++) {
        columns_name[i] = OS_NAME_ARRAY[index];
        columns_detail[i] = {name:'USER_COUNT_' + index, index:'USER_COUNT_' + index, width:70, align:"right", sortable:false, formatter:'integer'};
    }

    $('<table id="count_table"></table>').appendTo($('#grid-area')).jqGrid({
        url:'<?php echo site_url(array('log', 'os', 'ajaxGetActiveUserCountOfOSData')); ?>',
        mtype: "POST",
        postData:{ 'start_date':function() { return $('#start_date').val(); }, 'end_date':function() { return $('#end_date').val(); } },
        datatype: "json",
        colNames:columns_name,
        colModel:columns_detail,
        rowNum:-1,
        multiselect:true,
        shrinkToFit:false,
        viewrecords:true,
        sortorder:"desc",
        jsonReader:{
            repeatitems:false,
            root:"rows"
        },
        width:1000,
        height:"auto",
        //toolbar:[true,"top"],
        caption:"Active user count"
    });

    $("#count_table").jqGrid('setFrozenColumns');
    $("input","#t_count_table").click(function() { clickChartButton('User Count by Level', '#count_table'); });
}

function showRemainedUserCountOfOS() {
    $('#grid-area').empty();

    var columns_name = ['Date', 'Total'];
    var columns_detail = [
            {name:'STAT_DATE', index:'STAT_DATE', width:90, align:"center", sortable:false, frozen:true},
            {name:'TOTAL_COUNT', index:'TOTAL_COUNT', width:90, align:"right", sortable:false, frozen:true}
        ];

    for (var i = columns_name.length, index = 1; index < OS_NAME_ARRAY.length; i++, index++) {
        columns_name[i] = OS_NAME_ARRAY[index];
        columns_detail[i] = {name:'USER_COUNT_' + index, index:'USER_COUNT_' + index, width:70, align:"center", sortable:false};
    }

    $('<table id="count_table"></table>').appendTo($('#grid-area')).jqGrid({
        url:'<?php echo site_url(array('log', 'os', 'ajaxGetRemainedUserCountOfOSData')); ?>',
        mtype: "POST",
        postData:{ 'start_date':function() { return $('#start_date').val(); }, 'end_date':function() { return $('#end_date').val(); } },
        datatype: "json",
        colNames:columns_name,
        colModel:columns_detail,
        rowNum:-1,
        multiselect:true,
        shrinkToFit:false,
        viewrecords:true,
        sortorder:"desc",
        jsonReader:{
            repeatitems:false,
            root:"rows"
        },
        width:1000,
        height:"auto",
        //toolbar:[true,"top"],
        caption:"Remained user count"
    });

    $("#count_table").jqGrid('setFrozenColumns');
    $("input","#t_count_table").click(function() { clickChartButton('User Count by Level', '#count_table'); });
}

function showUserConnectedCountOfOS() {
    $('#grid-area').empty();

    var columns_name = ['Date', 'Sum'];
    var columns_detail = [
            {name:'STAT_DATE', index:'STAT_DATE', width:90, align:"center", sortable:false, frozen:true},
            {name:'TOTAL_COUNT', index:'TOTAL_COUNT', width:70, align:"right", sortable:false, formatter:'integer', frozen:true}
        ];

    for (var i = columns_name.length, index = 1; index < OS_NAME_ARRAY.length; i++, index++) {
        columns_name[i] = OS_NAME_ARRAY[index];
        columns_detail[i] = {name:'USER_COUNT_' + index, index:'USER_COUNT_' + index, width:70, align:"right", sortable:false, formatter:'integer'};
    }

    $('<table id="count_table"></table>').appendTo($('#grid-area')).jqGrid({
        url:'<?php echo site_url(array('log', 'os', 'ajaxGetUserConnectedCountOfOSData')); ?>',
        mtype: "POST",
        postData:{ 'start_date':function() { return $('#start_date').val(); }, 'end_date':function() { return $('#end_date').val(); } },
        datatype: "json",
        colNames:columns_name,
        colModel:columns_detail,
        rowNum:-1,
        multiselect:true,
        shrinkToFit:false,
        viewrecords:true,
        sortorder:"desc",
        jsonReader:{
            repeatitems:false,
            root:"rows"
        },
        width:1000,
        height:"auto",
        //toolbar:[true,"top"],
        caption:"User connected count"
    });

    $("#count_table").jqGrid('setFrozenColumns');
    $("input","#t_count_table").click(function() { clickChartButton('User Count by Level', '#count_table'); });
}

function showUserPlayTimeOfOS() {
    $('#grid-area').empty();

    var columns_name = ['Date', 'Total'];
    var columns_detail = [
            {name:'STAT_DATE', index:'STAT_DATE', width:90, align:"center", sortable:false, frozen:true},
            {name:'TOTAL_COUNT', index:'TOTAL_COUNT', width:100, align:"right", sortable:false, frozen:true}
        ];

    for (var i = columns_name.length, index = 1; index < OS_NAME_ARRAY.length; i++, index++) {
        columns_name[i] = OS_NAME_ARRAY[index];
        columns_detail[i] = {name:'USER_COUNT_' + index, index:'USER_COUNT_' + index, width:80, align:"center", sortable:false};
    }

    $('<table id="count_table"></table>').appendTo($('#grid-area')).jqGrid({
        url:'<?php echo site_url(array('log', 'os', 'ajaxGetUserPlayTimeOfOSData')); ?>',
        mtype: "POST",
        postData:{ 'start_date':function() { return $('#start_date').val(); }, 'end_date':function() { return $('#end_date').val(); } },
        datatype: "json",
        colNames:columns_name,
        colModel:columns_detail,
        rowNum:-1,
        multiselect:true,
        shrinkToFit:false,
        viewrecords:true,
        sortorder:"desc",
        jsonReader:{
            repeatitems:false,
            root:"rows"
        },
        width:1000,
        height:"auto",
        //toolbar:[true,"top"],
        caption:"User play time (minutes)"
    });

    $("#count_table").jqGrid('setFrozenColumns');
    $("input","#t_count_table").click(function() { clickChartButton('User Count by Level', '#count_table'); });
}

$(function() {
    $('#chart-viewer').dialog({
        autoOpen:false,
        width:1000,
        height:700,
        modal:true,
        resizable:false,
        buttons:{
            "Close": function() {
                $(this).dialog('close');
            }
        },
        open:function() {
        },
        close:function() {
        }
    });
});
</script>

<div class="well">
  Period :
  <input type="text" name="start_date" id="start_date" class="date" size="8" readonly="readonly" />
  ~
  <input type="text" name="end_date" id="end_date" class="date" size="8" readonly="readonly" />

  <ul class="nav nav-pills">
    <li><a href="javascript:;" onclick="showUserJoinedCountOfOS();">User joined count</a></li>
    <li><a href="javascript:;" onclick="showActiveUserCountOfOS();">Active user count</a></li>
    <li><a href="javascript:;" onclick="showRemainedUserCountOfOS();">Remained user count</a></li>
    <li><a href="javascript:;" onclick="showUserConnectedCountOfOS();">User connected count</a></li>
    <li><a href="javascript:;" onclick="showUserPlayTimeOfOS();">User play time</a></li>
  </ul>

  <div id="grid-area"></div>
</div>

<div id="chart-viewer" title="Graph" style="display:none; margin:0px;">
  <div id="chart_div"></div>
</div>