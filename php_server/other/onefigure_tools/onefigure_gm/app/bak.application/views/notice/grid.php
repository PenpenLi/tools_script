<script type="text/javascript">
$(function() {
    $('#grid_table').jqGrid({
        url:'<?php echo site_url(array($this->uri->segment(1), 'notice', 'ajaxGetNotice')); ?>',
        editurl:'<?php echo site_url(array($this->uri->segment(1), 'notice', 'ajaxSetNotice')); ?>',
        mtype:'POST',
        datatype:'json',
        colNames:['No', 'Title', 'Content', 'Start Date', 'Finish Date', 'Show', 'Created Date'],
        colModel:[
            {name:'NOTICE_ID', index:'NOTICE_ID', width:50, align:'center'},
            {name:'TITLE', index:'TITLE', width:250, editable:true, editrules:{required:true}, editoptions:{size:50,maxlength:100}},
            {name:'CONTENT', index:'CONTENT', width:0, hidden:true, editable:true, editrules:{edithidden:true, required:true}, edittype:'textarea', editoptions:{'rows':5, 'cols':50}},
            {name:'START_DATE', index:'START_DATE', width:150, align:'center', editable:true, editrules:{required:true}, editoptions:{defaultValue:'<?php echo date('Y-m-d H:i:s'); ?>'}},
            {name:'END_DATE', index:'END_DATE', width:150, align:'center', editable:true, editrules:{required:true}, editoptions:{defaultValue:'<?php echo date('Y-m-d H:i:s', strtotime('+1 day')); ?>'}},
            {name:'IS_SHOW_NAME', index:'IS_SHOW_NAME', width:60, align:'center', editable:true, editrules:{required:true}, edittype:'select', editoptions:{name:'IS_SHOW', value:'1:Show;0:Hide'}},
            {name:'CREATE_DATE', index:'CREATE_DATE', width:150, align:'center'}
        ],
        rowNum:20,
        rowList:[20,50,100],
        pager:'#pager',
        jsonReader:{
            repeatitems:false,
            root:"rows",
            id:'NOTICE_ID'
        },
        viewrecords:true,
        sortorder:'desc',
        height:'100%',
        caption:'Notices'
    });

    $('#grid_table').jqGrid(
        'navGrid',
        '#pager',
        {edit:true, add:true, del:false},
        {width:500, height:290, reloadAfterSubmit:false, jqModal:true, closeOnEscape:true}, // edit options
        {width:500, height:290, reloadAfterSubmit:false, jqModal:true, closeOnEscape:true, closeAfterAdd:true}, // add options
        {reloadAfterSubmit:false, jqModal:true, closeOnEscape:true} // del options
    );
});
</script>

<div id="grid">
<table id="grid_table"></table>
<div id="pager"></div>
</div>
