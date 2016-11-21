<div class="row small">
    <div class="col-md-12">
        <div class="panel panel-default">
            <div class="panel-heading">
                <a href="<?php echo site_url(array('event', 'edit')); ?>" class="btn btn-primary">Add</a>
            </div>

            <table class="table table-bordered table-responsive">
                <colgroup>
                    <col width="60" />
                    <col width="100" />
                    <col />
                    <col width="200" />
                    <col width="80" />
                </colgroup>

                <thead>
                <tr>
                    <th class="text-center">ID</th>
                    <th class="text-center">Process</th>
                    <th class="text-center">Title</th>
                    <th class="text-center">Period of activated</th>
                    <th class="text-center">User Count</th>
                </tr>
                </thead>

                <tbody>
                <?php
                foreach ($EVENT_ROWS as $row) {
                    ?>
                    <tr>
                        <td class="text-center"><?php echo $row['EVENT_ID']; ?></td>
                        <td class="text-center"><?php echo $row['PROCESS_NAME']; ?></td>
                        <td><a href="<?php echo site_url(array('event', 'edit', $row['EVENT_ID'])); ?>"><?php echo $row['TITLE']; ?></a></td>
                        <td class="text-center"><?php echo date('Y-m-d', strtotime($row['START_DATE'])); ?> ~ <?php echo date('Y-m-d', strtotime($row['END_DATE'])); ?></td>
                        <td class="text-center"><?php echo number_format($row['USER_COUNT']); ?></td>
                    </tr>
                <?php } ?>
                </tbody>
            </table>
        </div>
    </div>
</div>