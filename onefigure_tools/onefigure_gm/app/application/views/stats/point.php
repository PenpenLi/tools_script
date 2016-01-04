<script type="text/javascript">
    google.load("visualization", "1", {packages:["corechart"]});

    var CATEGORY_ARRAY = <?php echo json_encode($CATEGORY_ARRAY); ?>;

    function showPointGainGrid() {
        var COLUMN_MODEL = [
            {label:'날짜', name:'DATA_DATE', index:'DATA_DATE', width:90, align:"center", frozen:true},
        ];
        var GROUP_HEADER = [];

        for (var index in CATEGORY_ARRAY) {
            if (index.substring(0, 1) == '-') continue;

            COLUMN_MODEL.push({label:'은화', name:'VALUE_1_' + index, index:'VALUE_1_' + index, width:80, align:"right", formatter:'integer'});
            COLUMN_MODEL.push({label:'금화', name:'VALUE_2_' + index, index:'VALUE_2_' + index, width:80, align:"right", formatter:'integer'});
            GROUP_HEADER.push({"numberOfColumns": 2, "titleText":CATEGORY_ARRAY[index], "startColumnName":'VALUE_1_' + index});
        }

        $('#grid-area').empty().append($('<table id="grid_table"></table>'));
        $('#grid_table').jqGrid({
            url:"<?php echo site_url(array('stats', 'ajaxSelectPointStat')); ?>",
            mtype: "POST",
            postData:{ 'start_date':function() { return $('#start_date').val(); }, 'end_date':function() { return $('#end_date').val(); } },
            datatype: "json",
            colModel:COLUMN_MODEL,
            rowNum:-1,
            multiselect:true,
            shrinkToFit:false,
            viewrecords:true,
            sortorder:"desc",
            width:900,
            height:"auto",
            toolbar:[false,"top"],
            caption:"은화, 금화 획득"
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

    function showPointUseGrid() {
        var COLUMN_MODEL = [
            {label:'날짜', name:'DATA_DATE', index:'DATA_DATE', width:90, align:"center", frozen:true},
        ];
        var GROUP_HEADER = [];

        for (var index in CATEGORY_ARRAY) {
            if (index.substring(0, 1) != '-') continue;

            COLUMN_MODEL.push({label:'은화', name:'VALUE_1_' + index, index:'VALUE_1_' + index, width:80, align:"right", formatter:'integer'});
            COLUMN_MODEL.push({label:'금화', name:'VALUE_2_' + index, index:'VALUE_2_' + index, width:80, align:"right", formatter:'integer'});
            GROUP_HEADER.push({"numberOfColumns": 2, "titleText":CATEGORY_ARRAY[index], "startColumnName":'VALUE_1_' + index});
        }

        $('#grid-area').empty().append($('<table id="grid_table"></table>'));
        $('#grid_table').jqGrid({
            url:"<?php echo site_url(array('stats', 'ajaxSelectPointStat')); ?>",
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
            caption:"은화, 금화 획득"
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
            <li role="presentation"><a href="javascript:;" onclick="showPointGainGrid();">은화, 금화 획득</a></li>
            <li role="presentation"><a href="javascript:;" onclick="showPointUseGrid();">은화, 금화 사용</a></li>
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