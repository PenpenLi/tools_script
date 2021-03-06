<script type="text/javascript">
    google.load("visualization", "1", {packages:["corechart"]});

    var STAGE_ARRAY = <?php echo json_encode($STAGE_ARRAY); ?>;
    var CHARACTER_ARRAY = <?php echo json_encode($CHARACTER_ARRAY); ?>;
    var SKILL_ARRAY = <?php echo json_encode($SKILL_ARRAY); ?>;
    var TREASURE_ARRAY = <?php echo json_encode($TREASURE_ARRAY); ?>;

    function showStageGrid() {
        var COLUMN_MODEL = [
            {label:'날짜', name:'DATA_DATE', index:'DATA_DATE', width:90, align:"center", frozen:true},
        ];
        var GROUP_HEADER = [];

        for (var index in STAGE_ARRAY) {
            COLUMN_MODEL.push({label:'[' + index + '] ' + STAGE_ARRAY[index]['GAME_MODE'], name:'VALUE_' + index, index:'VALUE_' + index, width:100, align:"right", formatter:'integer'});
        }

        $('#grid-area').empty().append($('<table id="grid_table"></table>'));
        $('#grid_table').jqGrid({
            url:"<?php echo site_url(array('stats', 'ajaxSelectStageStat')); ?>",
            mtype:"POST",
            postData:{ 'start_date':function() { return $('#start_date').val(); }, 'end_date':function() { return $('#end_date').val(); } },
            datatype:"json",
            colModel:COLUMN_MODEL,
            rowNum:-1,
            multiselect:true,
            shrinkToFit:false,
            viewrecords:true,
            sortorder:"desc",
            width:900,
            height:"auto",
            toolbar:[false,"top"],
            caption:"스테이지 현황"
        });
        $('#grid_table').jqGrid('setFrozenColumns');

        $('#t_grid_table')
            .append('<input type="button" id="click_graph" value="Open Graph Choose Rows"/>')
            .append('<input type="button" id="click_excel" value="Download Excel"/>');
        $("#click_graph","#t_grid_table").click(function() { clickChartButton(title, 'grid_table'); });
        $("#click_excel","#t_grid_table").click(function() { clickExcelButton(excel_url); });
    }

    function showCharacterGrid() {
        var COLUMN_MODEL = [
            {label:'날짜', name:'DATA_DATE', index:'DATA_DATE', width:90, align:"center", frozen:true},
        ];
        var GROUP_HEADER = [];

        for (var index in CHARACTER_ARRAY) {
            for (var i = 1; i <= 8; i++) {
                COLUMN_MODEL.push({
                    label: i,
                    name: 'VALUE_' + index + '_' + i,
                    index: 'VALUE_' + index + '_' + i,
                    width: 80,
                    align: "right",
                    formatter: 'integer'
                });
            }

            GROUP_HEADER.push({"numberOfColumns":8, "titleText":'권법 ' + index, "startColumnName":'VALUE_' + index + '_1'});
        }

        $('#grid-area').empty().append($('<table id="grid_table"></table>'));
        $('#grid_table').jqGrid({
            url:"<?php echo site_url(array('stats', 'ajaxSelectCharacterStat')); ?>",
            mtype: "POST",
            postData:{ 'start_date':function() { return $('#start_date').val(); }, 'end_date':function() { return $('#end_date').val(); } },
            datatype: "json",
            colModel:COLUMN_MODEL,
            rowNum:-1,
            multiselect: true,
            shrinkToFit:false,
            viewrecords:true,
            sortorder:"desc",
            width:900,
            height:"auto",
            toolbar:[false,"top"],
            caption:"권법 보유 현황"
        });
        $('#grid_table').setGroupHeaders({
            useColSpanStyle:true,
            groupHeaders:GROUP_HEADER
        });
        $('#grid_table').jqGrid('setFrozenColumns');

        $('#t_grid_table')
            .append('<input type="button" id="click_graph" value="Open Graph Choose Rows"/>')
            .append('<input type="button" id="click_excel" value="Download Excel"/>');
        $("#click_graph","#t_grid_table").click(function() { clickChartButton(title, 'grid_table'); });
        $("#click_excel","#t_grid_table").click(function() { clickExcelButton(excel_url); });
    }

    function showSkillGrid() {
        var COLUMN_MODEL = [
            {label:'날짜', name:'DATA_DATE', index:'DATA_DATE', width:90, align:"center", frozen:true},
        ];
        var GROUP_HEADER = [];

        for (var index in SKILL_ARRAY) {
            for (var i = 1; i <= 8; i++) {
                COLUMN_MODEL.push({
                    label: i,
                    name: 'VALUE_' + index + '_' + i,
                    index: 'VALUE_' + index + '_' + i,
                    width: 80,
                    align: "right",
                    formatter: 'integer'
                });
            }

            GROUP_HEADER.push({"numberOfColumns":8, "titleText":SKILL_ARRAY[index]['NAME'], "startColumnName":'VALUE_' + index + '_1'});
        }

        $('#grid-area').empty().append($('<table id="grid_table"></table>'));
        $('#grid_table').jqGrid({
            url:"<?php echo site_url(array('stats', 'ajaxSelectSkillStat')); ?>",
            mtype: "POST",
            postData:{ 'start_date':function() { return $('#start_date').val(); }, 'end_date':function() { return $('#end_date').val(); } },
            datatype: "json",
            colModel:COLUMN_MODEL,
            rowNum:-1,
            multiselect: true,
            shrinkToFit:false,
            viewrecords:true,
            sortorder:"desc",
            width:900,
            height:"auto",
            toolbar:[false,"top"],
            caption:"스킬 보유 현황"
        });
        $('#grid_table').setGroupHeaders({
            useColSpanStyle:true,
            groupHeaders:GROUP_HEADER
        });
        $('#grid_table').jqGrid('setFrozenColumns');

        $('#t_grid_table')
            .append('<input type="button" id="click_graph" value="Open Graph Choose Rows"/>')
            .append('<input type="button" id="click_excel" value="Download Excel"/>');
        $("#click_graph","#t_grid_table").click(function() { clickChartButton(title, 'grid_table'); });
        $("#click_excel","#t_grid_table").click(function() { clickExcelButton(excel_url); });
    }

    function showTreasureGrid() {
        var COLUMN_MODEL = [
            {label:'날짜', name:'DATA_DATE', index:'DATA_DATE', width:90, align:"center", frozen:true},
        ];
        var GROUP_HEADER = [];

        for (var index in TREASURE_ARRAY) {
            for (var i = 1; i <= 8; i++) {
                COLUMN_MODEL.push({
                    label: i,
                    name: 'VALUE_' + index + '_' + i,
                    index: 'VALUE_' + index + '_' + i,
                    width: 80,
                    align: "right",
                    formatter: 'integer'
                });
            }

            GROUP_HEADER.push({"numberOfColumns":8, "titleText":TREASURE_ARRAY[index]['NAME'], "startColumnName":'VALUE_' + index + '_1'});
        }

        $('#grid-area').empty().append($('<table id="grid_table"></table>'));
        $('#grid_table').jqGrid({
            url:"<?php echo site_url(array('stats', 'ajaxSelectTreasureStat')); ?>",
            mtype: "POST",
            postData:{ 'start_date':function() { return $('#start_date').val(); }, 'end_date':function() { return $('#end_date').val(); } },
            datatype: "json",
            colModel:COLUMN_MODEL,
            rowNum:-1,
            multiselect: true,
            shrinkToFit:false,
            viewrecords:true,
            sortorder:"desc",
            width:900,
            height:"auto",
            toolbar:[false,"top"],
            caption:"장비 보유 현황"
        });
        $('#grid_table').setGroupHeaders({
            useColSpanStyle:true,
            groupHeaders:GROUP_HEADER
        });
        $('#grid_table').jqGrid('setFrozenColumns');

        $('#t_grid_table')
            .append('<input type="button" id="click_graph" value="Open Graph Choose Rows"/>')
            .append('<input type="button" id="click_excel" value="Download Excel"/>');
        $("#click_graph","#t_grid_table").click(function() { clickChartButton(title, 'grid_table'); });
        $("#click_excel","#t_grid_table").click(function() { clickExcelButton(excel_url); });
    }

    function showPlayCountGrid() {
        var COLUMN_MODEL = [
            {label:'날짜', name:'DATA_DATE', index:'DATA_DATE', width:90, align:"center", frozen:true},
            {label:'합계', name:'TOTAL_COUNT', index:'TOTAL_COUNT', width:100, align:"right", formatter:'integer', frozen:true},
            {label:'서바이벌', name:'VALUE_0', index:'VALUE_0', width:100, align:"right", formatter:'integer'}
        ];
        var GROUP_HEADER = [];

        for (var index in STAGE_ARRAY) {
            COLUMN_MODEL.push({label:'[' + index + '] ' + STAGE_ARRAY[index]['GAME_MODE'], name:'VALUE_' + index, index:'VALUE_' + index, width:100, align:"right", formatter:'integer'});
        }

        $('#grid-area').empty().append($('<table id="grid_table"></table>'));
        $('#grid_table').jqGrid({
            url:"<?php echo site_url(array('stats', 'ajaxSelectPlayCountStat')); ?>",
            mtype:"POST",
            postData:{ 'start_date':function() { return $('#start_date').val(); }, 'end_date':function() { return $('#end_date').val(); } },
            datatype:"json",
            colModel:COLUMN_MODEL,
            rowNum:-1,
            multiselect:true,
            shrinkToFit:false,
            viewrecords:true,
            sortorder:"desc",
            width:900,
            height:"auto",
            toolbar:[false,"top"],
            caption:"게임 플레이 수"
        });
        $('#grid_table').jqGrid('setFrozenColumns');

        $('#t_grid_table')
            .append('<input type="button" id="click_graph" value="Open Graph Choose Rows"/>')
            .append('<input type="button" id="click_excel" value="Download Excel"/>');
        $("#click_graph","#t_grid_table").click(function() { clickChartButton(title, 'grid_table'); });
        $("#click_excel","#t_grid_table").click(function() { clickExcelButton(excel_url); });
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

<div class="row small">
    <div class="col-sm-2">
        <ul class="nav nav-pills nav-stacked">
            <li role="presentation"><a href="javascript:;" onclick="showStageGrid();">스테이지 현황</a></li>
            <li role="presentation"><a href="javascript:;" onclick="showCharacterGrid();">권법 보유 현황</a></li>
            <li role="presentation"><a href="javascript:;" onclick="showSkillGrid();">스킬 보유 현황</a></li>
            <li role="presentation"><a href="javascript:;" onclick="showTreasureGrid();">장비 보유 현황</a></li>
            <li role="presentation"><a href="javascript:;" onclick="showPlayCountGrid();">게임 플레이 수</a></li>
        </ul>
    </div>
    <div class="col-sm-10">
        <div class="panel panel-default">
            <div class="panel-heading">
                <form class="form-inline">
                    <div class="form-group">
                        검색기간 :
                        <input type="text" name="start_date" id="start_date" class="date form-control" size="8" readonly="readonly" />
                        ~
                        <input type="text" name="end_date" id="end_date" class="date form-control" size="8" readonly="readonly" />
                    </div>
                </form>
            </div>
            <div class="panel-body">
                <div id="grid-area"></div>
            </div>
        </div>

        <div id="chart-viewer" title="Graph" style="display:none; margin:0px;">
            <div id="chart_div"></div>
        </div>
    </div>
</div>