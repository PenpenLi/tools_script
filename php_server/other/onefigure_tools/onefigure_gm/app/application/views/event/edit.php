<script type="text/javascript">
    $(function() {
        var dates = $('.period_date').datepicker({
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
            }
        });

        $('#form-reward-type').change(function() {
            switch ($(this).val()) {
                case '3' :
                case '4' :
                case '5' :
                case '8' :
                    $('input[name=reward_item_id]').removeAttr('disabled');
                    break;

                default :
                    $('input[name=reward_item_id]').attr('disabled', 'disabled');
            }
        }).change();

        $('#form-spot-time').change(function() {
            if ($(this).is(':checked')) {
                $('select[name=spot_start_time]').removeAttr('disabled');
                $('select[name=spot_end_time]').removeAttr('disabled');
            } else {
                $('select[name=spot_start_time]').attr('disabled', 'disabled');
                $('select[name=spot_end_time]').attr('disabled', 'disabled');
            }
        }).change();

        $('#form-event-edit').validate();
    });
</script>
<div class="row small">
    <div class="col-md-2"></div>
    <div class="col-md-8">
        <div class="panel panel-default">
                <div class="panel-heading">Event Edit</div>
            <form class="form-horizontal" role="form" id="form-event-edit" name="form_event_edit" method="post" action="<?php echo site_url(array('event', 'execSetEvent')); ?>">
                <input type="hidden" name="event_id" value="<?php echo $EVENT_ROW['EVENT_ID']; ?>" />

                <div class="panel-body">
                    <div class="form-group">
                        <label for="form-title" class="col-sm-2 control-label">Title</label>
                        <div class="col-sm-10">
                            <input type="text" id="form-title" name="title" class="form-control" value="<?php echo $EVENT_ROW['TITLE']; ?>" placeholder="Title" required="required" />
                        </div>
                    </div>
                    <div class="form-group form-inline">
                        <label for="form-process" class="col-sm-2 control-label">Process</label>
                        <div class="col-sm-10">
                            <select id="form-process" name="process" class="form-control" min="1" required="required">
                                <option value="">Process type</option>
                                <option value="1" <?php echo $EVENT_ROW['PROCESS_CODE'] == 1 ? 'selected="selected"' : ''; ?>>Login (Lobby)</option>
                                <option value="2" <?php echo $EVENT_ROW['PROCESS_CODE'] == 2 ? 'selected="selected"' : ''; ?>>Game start</option>
                                <option value="3" <?php echo $EVENT_ROW['PROCESS_CODE'] == 3 ? 'selected="selected"' : ''; ?>>Game finish</option>
                                <option value="4" <?php echo $EVENT_ROW['PROCESS_CODE'] == 4 ? 'selected="selected"' : ''; ?>>Payment</option>
                            </select>
                        </div>
                    </div>
                    <div class="form-group form-inline">
                        <label for="form-start-date" class="col-sm-2 control-label">Period</label>
                        <div class="col-sm-10">
                            <input type="date" id="form-start-date" name="start_date" class="form-control period_date" value="<?php echo date('Y-m-d', strtotime(isset($EVENT_ROW['START_DATE']) ? $EVENT_ROW['START_DATE'] : 'today')); ?>" placeholder="Start" required="required" />
                            ~
                            <input type="date" id="form-end-date" name="end_date" class="form-control period_date" value="<?php echo date('Y-m-d', strtotime(isset($EVENT_ROW['END_DATE']) ? $EVENT_ROW['END_DATE'] : '+5 days')); ?>" placeholder="End" required="required" />
                        </div>
                    </div>
                    <div class="form-group form-inline">
                        <label for="form-spot-time" class="col-sm-2 control-label">Spot time</label>
                        <div class="col-sm-10">
                            <input type="checkbox" id="form-spot-time" class="form-control" name="spot_enable" value="1" <?php echo !is_null($EVENT_ROW['SPOT_START_TIME']) > 0 ? 'checked="checked"' : ''; ?> />
                            <select name="spot_start_time" class="form-control" disabled="disabled">
                                <option value="">Start time</option>
                                <?php for ($i = 0; $i < 24; $i++) { ?>
                                    <option value="<?php echo $i; ?>" <?php echo $EVENT_ROW['SPOT_START_TIME'] === $i ? 'selected="selected"' : ''; ?>><?php echo sprintf('%02d', $i); ?>:00</option>
                                <?php } ?>
                            </select>
                            ~
                            <select name="spot_end_time" class="form-control" disabled="disabled">
                                <option value="">Finish time</option>
                                <?php for ($i = 0; $i < 24; $i++) { ?>
                                    <option value="<?php echo $i; ?>" <?php echo $EVENT_ROW['SPOT_END_TIME'] === $i ? 'selected="selected"' : ''; ?>><?php echo sprintf('%02d', $i); ?>:59</option>
                                <?php } ?>
                            </select>
                        </div>
                    </div>
                    <div class="form-group form-inline">
                        <label for="form-repeat-type" class="col-sm-2 control-label">Repeat</label>
                        <div class="col-sm-10">
                            <select id="form-repeat-type" name="repeat_type" class="form-control" min="1" required="required">
                                <option value="">Repeat type</option>
                                <option value="1" <?php echo $EVENT_ROW['REPEAT_CODE'] == 1 ? 'selected="selected"' : ''; ?>>a day</option>
                                <option value="2" <?php echo $EVENT_ROW['REPEAT_CODE'] == 2 ? 'selected="selected"' : ''; ?>>in period</option>
                            </select>
                            <input type="number" name="repeat_count" class="form-control" value="<?php echo $EVENT_ROW['REPEAT_COUNT']; ?>" placeholder="Count" min="1" required="required" data-toggle="tooltip" data-placement="bottom" title="Repeat count" />
                            <input type="number" name="repeat_limit" class="form-control" value="<?php echo $EVENT_ROW['LIMIT_COUNT']; ?>" placeholder="Limit Count" min="0" required="required" data-toggle="tooltip" data-placement="bottom" title="Limit count (0=Unlimited)" />
                        </div>
                    </div>
                    <div class="form-group form-inline">
                        <label for="form-reward-type" class="col-sm-2 control-label">Reward</label>
                        <div class="col-sm-10">
                            <select id="form-reward-type" name="reward_type" class="form-control" min="0" required="required">
                                <option value="">Reward type</option>
                                <option value="0" <?php echo $EVENT_ROW['GIFT_TYPE'] == 0 ? 'selected="selected"' : ''; ?>>Money</option>
                                <option value="1" <?php echo $EVENT_ROW['GIFT_TYPE'] == 1 ? 'selected="selected"' : ''; ?>>Cash</option>
                                <option value="2" <?php echo $EVENT_ROW['GIFT_TYPE'] == 2 ? 'selected="selected"' : ''; ?>>Stamina</option>
                                <option value="3" <?php echo $EVENT_ROW['GIFT_TYPE'] == 3 ? 'selected="selected"' : ''; ?>>Mystique</option>
                                <option value="4" <?php echo $EVENT_ROW['GIFT_TYPE'] == 4 ? 'selected="selected"' : ''; ?>>Stance</option>
                                <option value="5" <?php echo $EVENT_ROW['GIFT_TYPE'] == 5 ? 'selected="selected"' : ''; ?>>Artifact</option>
                                <option value="6" <?php echo $EVENT_ROW['GIFT_TYPE'] == 6 ? 'selected="selected"' : ''; ?>>Present Box</option>
                                <option value="8" <?php echo $EVENT_ROW['GIFT_TYPE'] == 8 ? 'selected="selected"' : ''; ?>>Booster</option>
                                <option value="9" <?php echo $EVENT_ROW['GIFT_TYPE'] == 9 ? 'selected="selected"' : ''; ?>>High Ticket</option>
                                <option value="10" <?php echo $EVENT_ROW['GIFT_TYPE'] == 10 ? 'selected="selected"' : ''; ?>>Ticket</option>
                            </select>
                            <input type="number" name="reward_item_id" class="form-control" value="<?php echo $EVENT_ROW['ITEM_ID']; ?>" placeholder="Item ID" min="1" data-toggle="tooltip" data-placement="bottom" title="Item ID" disabled="disabled" />
                            <input type="number" name="reward_amount" class="form-control" value="<?php echo $EVENT_ROW['AMOUNT']; ?>" placeholder="Amount" min="1" required="required" data-toggle="tooltip" data-placement="bottom" title="Amount" />
                        </div>
                    </div>
                </div>

                <div class="panel-footer">
                    <button type="submit" class="btn btn-primary">Save</button>
                    <button type="reset" class="btn btn-default">Cancel</button>
                </div>
            </form>
        </div>
    </div>
</div>