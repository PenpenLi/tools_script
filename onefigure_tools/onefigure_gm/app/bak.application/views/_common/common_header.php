<!DOCTYPE html>
<html>
<head>
    <title>::: One Punch Tool :::</title>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <script type="text/javascript" src="https://www.google.com/jsapi"></script>
    <script type="text/javascript" src="//ajax.aspnetcdn.com/ajax/jQuery/jquery-1.10.0.min.js"></script>
    <script type="text/javascript" src="//ajax.aspnetcdn.com/ajax/jquery.ui/1.10.4/jquery-ui.js"></script>
    <script type="text/javascript" src="//ajax.microsoft.com/ajax/jquery.templates/beta1/jquery.tmpl.min.js"></script>
    <script type="text/javascript" src="//ajax.aspnetcdn.com/ajax/jquery.validate/1.11.1/jquery.validate.min.js"></script>
    <script type="text/javascript" src="//ajax.aspnetcdn.com/ajax/jquery.validate/1.11.1/additional-methods.min.js"></script>
    <script type="text/javascript" src="//ajax.aspnetcdn.com/ajax/jquery.validate/1.11.1/localization/messages_ko.js"></script>
    <script type="text/javascript" src="//ajax.aspnetcdn.com/ajax/bootstrap/3.3.0/bootstrap.min.js"></script>
    <script type="text/javascript" src="/lib/js/jqGrid/4.7.0/js/i18n/grid.locale-en.js"></script>
    <script type="text/javascript" src="/lib/js/jqGrid/4.7.0/js/jquery.jqGrid.min.js"></script>
    <script type="text/javascript" src="/lib/js/jquery/jquery.cookie.js"></script>

    <link rel="stylesheet" href="//ajax.aspnetcdn.com/ajax/jquery.ui/1.10.4/themes/flick/jquery-ui.css" type="text/css" media="all" />
    <link rel="stylesheet" href="/lib/js/jqGrid/4.7.0/css/ui.jqgrid.css" type="text/css" media="all" />
    <link rel="stylesheet" href="//ajax.aspnetcdn.com/ajax/bootstrap/3.3.0/css/bootstrap.css" type="text/css" media="all" />
    <link rel="stylesheet" href="/lib/css/bootflat/css/bootflat.min.css" type="text/css" media="all" />

<script type="text/javascript">
/* constants variable - start */
document.domain = 'tangentkorea.com';
/* constants variable - end */


$(function() {
    var dates = $('.date').datepicker({
        dateFormat:'yy-mm-dd',
        showOtherMonths: true,
        selectOtherMonths: true,
        onSelect: function(selectedDate) {
            var option = $(this).attr('name') == "start_date" ? "minDate" : "maxDate",
                instance = $( this ).data( "datepicker" ),
                date = $.datepicker.parseDate(
                    instance.settings.dateFormat ||
                    $.datepicker._defaults.dateFormat,
                    selectedDate, instance.settings );
            dates.not( this ).datepicker( "option", option, date );

            if ($(this).attr('name') == "start_date") {
                $.cookie('START_DATE', selectedDate);
            } else if ($(this).attr('name') == "end_date") {
                $.cookie('END_DATE', selectedDate);
            }
        }
    });

    var START_DATE = $.cookie('START_DATE') || '<?php echo date('Y-m-d', strtotime('-7 days')); ?>';
    var END_DATE = $.cookie('END_DATE') || '<?php echo date('Y-m-d'); ?>';

    $('#start_date').datepicker('setDate', START_DATE);
    $('#end_date').datepicker('setDate', END_DATE);

    $('.nav-pills li')
        .click(function() {
            $('.nav-pills li').removeClass('active');
            $(this).addClass('active');
        });

    $('[data-toggle="tooltip"]').tooltip();
});
</script>

<style>
.start-date, .end-date {
    width:200px;
}
</style>
</head>

<body style="background-color:#f1f2f6;">
