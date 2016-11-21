<script type="text/javascript">
google.load("visualization", "1", {packages:["corechart"]});

function drawChart(title, data_array) {
    var data = google.visualization.arrayToDataTable(data_array);

    var options = {
        'title':title,
        'width':950,
        'height':550,
        'fontSize':11,
        'chartArea':{left:60,top:20,width:800,height:500}
    };

    var chart = new google.visualization.LineChart(document.getElementById('chart_div'));
    chart.draw(data, options);
}

function clickChartButton(title, grid_id) {
    var selected_rows_id = $("#" + grid_id).jqGrid('getGridParam', 'selarrrow');
    var selected_rows = null;

    selected_rows_id.sort();

    if (selected_rows_id.length == 0) {
        alert('데이터를 선택해 주세요.');
        return false;
    }

    var data_array = [['시간']];
    var date_index = 1;

    for (var i=0; i<24; i++) {
        data_array[(i + 1)] = [i + '시'];
    }

    for (var index in selected_rows_id) {
        selected_rows = $("#" + grid_id).jqGrid('getRowData', selected_rows_id[index]);
        data_array[0][date_index] = selected_rows_id[index];

        for (var i=0; i<24; i++) {
            data_array[i + 1][date_index] = parseInt(selected_rows['hour' + i]);
        }

        date_index++;
    }

    drawChart(title, data_array);
    $('#chart-viewer').dialog('open');
}

$(function() {
    $('#join_table').jqGrid({
         url:'<?php echo site_url(array($this->uri->segment(1), 'stats', 'ajaxGetStatsHourlyData', 1, YESTERDAY_MONTH)); ?>',
         mtype: "GET",
         datatype: "json",
         colNames:['Date', 'Sum', '0시', '1시', '2시', '3시', '4시', '5시', '6시', '7시', '8시', '9시', '10시', '11시', '12시', '13시', '14시', '15시', '16시', '17시', '18시', '19시', '20시', '21시', '22시', '23시'],
         colModel:[
            {name:'date', index:'date', width:90, align:"center", frozen:true},
            {name:'total', index:'total', width:80, align:"right", formatter:'integer', frozen:true},
            {name:'hour0', index:'hour0', width:70, align:"right", formatter:'integer'},
            {name:'hour1', index:'hour1', width:70, align:"right", formatter:'integer'},
            {name:'hour2', index:'hour2', width:70, align:"right", formatter:'integer'},
            {name:'hour3', index:'hour3', width:70, align:"right", formatter:'integer'},
            {name:'hour4', index:'hour4', width:70, align:"right", formatter:'integer'},
            {name:'hour5', index:'hour5', width:70, align:"right", formatter:'integer'},
            {name:'hour6', index:'hour6', width:70, align:"right", formatter:'integer'},
            {name:'hour7', index:'hour7', width:70, align:"right", formatter:'integer'},
            {name:'hour8', index:'hour8', width:70, align:"right", formatter:'integer'},
            {name:'hour9', index:'hour9', width:70, align:"right", formatter:'integer'},
            {name:'hour10', index:'hour10', width:70, align:"right", formatter:'integer'},
            {name:'hour11', index:'hour11', width:70, align:"right", formatter:'integer'},
            {name:'hour12', index:'hour12', width:70, align:"right", formatter:'integer'},
            {name:'hour13', index:'hour13', width:70, align:"right", formatter:'integer'},
            {name:'hour14', index:'hour14', width:70, align:"right", formatter:'integer'},
            {name:'hour15', index:'hour15', width:70, align:"right", formatter:'integer'},
            {name:'hour16', index:'hour16', width:70, align:"right", formatter:'integer'},
            {name:'hour17', index:'hour17', width:70, align:"right", formatter:'integer'},
            {name:'hour18', index:'hour18', width:70, align:"right", formatter:'integer'},
            {name:'hour19', index:'hour19', width:70, align:"right", formatter:'integer'},
            {name:'hour20', index:'hour20', width:70, align:"right", formatter:'integer'},
            {name:'hour21', index:'hour21', width:70, align:"right", formatter:'integer'},
            {name:'hour22', index:'hour22', width:70, align:"right", formatter:'integer'},
            {name:'hour23', index:'hour23', width:70, align:"right", formatter:'integer'}
        ],
        rowNum:31,
        multiselect: true,
        shrinkToFit:false,
        viewrecords:true,
        sortorder:"desc",
        width:1000,
        height:"auto",
        toolbar:[true,"top"],
        caption:"시간당 가입자수"
    });

    $('#play_table').jqGrid({
         url:'<?php echo site_url(array($this->uri->segment(1), 'stats', 'ajaxGetStatsHourlyData', 2, YESTERDAY_MONTH)); ?>',
         mtype: "GET",
         datatype: "json",
         colNames:['Date', 'Sum', '0시', '1시', '2시', '3시', '4시', '5시', '6시', '7시', '8시', '9시', '10시', '11시', '12시', '13시', '14시', '15시', '16시', '17시', '18시', '19시', '20시', '21시', '22시', '23시'],
         colModel:[
            {name:'date', index:'date', width:90, align:"center", frozen:true},
            {name:'total', index:'total', width:80, align:"right", formatter:'integer', frozen:true},
            {name:'hour0', index:'hour0', width:70, align:"right", formatter:'integer'},
            {name:'hour1', index:'hour1', width:70, align:"right", formatter:'integer'},
            {name:'hour2', index:'hour2', width:70, align:"right", formatter:'integer'},
            {name:'hour3', index:'hour3', width:70, align:"right", formatter:'integer'},
            {name:'hour4', index:'hour4', width:70, align:"right", formatter:'integer'},
            {name:'hour5', index:'hour5', width:70, align:"right", formatter:'integer'},
            {name:'hour6', index:'hour6', width:70, align:"right", formatter:'integer'},
            {name:'hour7', index:'hour7', width:70, align:"right", formatter:'integer'},
            {name:'hour8', index:'hour8', width:70, align:"right", formatter:'integer'},
            {name:'hour9', index:'hour9', width:70, align:"right", formatter:'integer'},
            {name:'hour10', index:'hour10', width:70, align:"right", formatter:'integer'},
            {name:'hour11', index:'hour11', width:70, align:"right", formatter:'integer'},
            {name:'hour12', index:'hour12', width:70, align:"right", formatter:'integer'},
            {name:'hour13', index:'hour13', width:70, align:"right", formatter:'integer'},
            {name:'hour14', index:'hour14', width:70, align:"right", formatter:'integer'},
            {name:'hour15', index:'hour15', width:70, align:"right", formatter:'integer'},
            {name:'hour16', index:'hour16', width:70, align:"right", formatter:'integer'},
            {name:'hour17', index:'hour17', width:70, align:"right", formatter:'integer'},
            {name:'hour18', index:'hour18', width:70, align:"right", formatter:'integer'},
            {name:'hour19', index:'hour19', width:70, align:"right", formatter:'integer'},
            {name:'hour20', index:'hour20', width:70, align:"right", formatter:'integer'},
            {name:'hour21', index:'hour21', width:70, align:"right", formatter:'integer'},
            {name:'hour22', index:'hour22', width:70, align:"right", formatter:'integer'},
            {name:'hour23', index:'hour23', width:70, align:"right", formatter:'integer'}
        ],
        rowNum:31,
        multiselect: true,
        shrinkToFit:false,
        viewrecords:true,
        sortorder:"desc",
        width:1000,
        height:"auto",
        toolbar:[true,"top"],
        caption:"시간당 게임 플레이수"
    });

    $('#mail_send_table').jqGrid({
         url:'<?php echo site_url(array($this->uri->segment(1), 'stats', 'ajaxGetStatsHourlyData', 3, YESTERDAY_MONTH)); ?>',
         mtype: "GET",
         datatype: "json",
         colNames:['Date', 'Sum', '0시', '1시', '2시', '3시', '4시', '5시', '6시', '7시', '8시', '9시', '10시', '11시', '12시', '13시', '14시', '15시', '16시', '17시', '18시', '19시', '20시', '21시', '22시', '23시'],
         colModel:[
            {name:'date', index:'date', width:90, align:"center", frozen:true},
            {name:'total', index:'total', width:80, align:"right", formatter:'integer', frozen:true},
            {name:'hour0', index:'hour0', width:70, align:"right", formatter:'integer'},
            {name:'hour1', index:'hour1', width:70, align:"right", formatter:'integer'},
            {name:'hour2', index:'hour2', width:70, align:"right", formatter:'integer'},
            {name:'hour3', index:'hour3', width:70, align:"right", formatter:'integer'},
            {name:'hour4', index:'hour4', width:70, align:"right", formatter:'integer'},
            {name:'hour5', index:'hour5', width:70, align:"right", formatter:'integer'},
            {name:'hour6', index:'hour6', width:70, align:"right", formatter:'integer'},
            {name:'hour7', index:'hour7', width:70, align:"right", formatter:'integer'},
            {name:'hour8', index:'hour8', width:70, align:"right", formatter:'integer'},
            {name:'hour9', index:'hour9', width:70, align:"right", formatter:'integer'},
            {name:'hour10', index:'hour10', width:70, align:"right", formatter:'integer'},
            {name:'hour11', index:'hour11', width:70, align:"right", formatter:'integer'},
            {name:'hour12', index:'hour12', width:70, align:"right", formatter:'integer'},
            {name:'hour13', index:'hour13', width:70, align:"right", formatter:'integer'},
            {name:'hour14', index:'hour14', width:70, align:"right", formatter:'integer'},
            {name:'hour15', index:'hour15', width:70, align:"right", formatter:'integer'},
            {name:'hour16', index:'hour16', width:70, align:"right", formatter:'integer'},
            {name:'hour17', index:'hour17', width:70, align:"right", formatter:'integer'},
            {name:'hour18', index:'hour18', width:70, align:"right", formatter:'integer'},
            {name:'hour19', index:'hour19', width:70, align:"right", formatter:'integer'},
            {name:'hour20', index:'hour20', width:70, align:"right", formatter:'integer'},
            {name:'hour21', index:'hour21', width:70, align:"right", formatter:'integer'},
            {name:'hour22', index:'hour22', width:70, align:"right", formatter:'integer'},
            {name:'hour23', index:'hour23', width:70, align:"right", formatter:'integer'}
        ],
        rowNum:31,
        multiselect: true,
        shrinkToFit:false,
        viewrecords:true,
        sortorder:"desc",
        width:1000,
        height:"auto",
        toolbar:[true,"top"],
        caption:"시간당 하트 발송수"
    });

    $(".grid_table").jqGrid('setFrozenColumns');
    $("#t_join_table,#t_play_table,#t_mail_send_table").append('<input type="button" value="Open Graph Choose Rows"/>');
    $("input","#t_join_table").click(function() { clickChartButton('시간당 가입자수', 'join_table'); });
    $("input","#t_play_table").click(function() { clickChartButton('시간당 게임 플레이수', 'play_table'); });
    $("input","#t_mail_send_table").click(function() { clickChartButton('시간당 하트 발송수', 'mail_send_table'); });

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

<table id="join_table" class="grid_table"></table>
<br />
<table id="play_table" class="grid_table"></table>
<br />
<table id="mail_send_table" class="grid_table"></table>

<div id="chart-viewer" title="Graph" style="display:none; margin:0px;">
  <div id="chart_div"></div>
</div>