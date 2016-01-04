<script type="text/javascript">
$(function() {
    $('#find-form').submit(function () {
        var find_keyword = $('#find_keyword');

        if ($.trim(find_keyword.val()) == '') {
            alert('empty keyword');
            find_keyword.focus();
            return;
        }

        location.href = $(this).attr('action') + '/' + encodeURIComponent(find_keyword.val());
        return false;
    });
});
</script>

<div id="wrap">
  <div class="navbar navbar-default" role="navigation">
    <div class="container">
      <div class="navbar-header">
        <button type="button" class="navbar-toggle" data-toggle="collapse" data-target="#bs-example-navbar-collapse-1">
          <span class="sr-only">Toggle navigation</span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
        </button>
        <a class="navbar-brand" href="/">One Punch</a>
      </div>

      <div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
       
        <?php echo form_open(site_url(array('user', 'find')), array('method' => 'get', 'id' => 'find-form', 'class' => 'navbar-form navbar-left', 'role' => 'search')); ?>
          <div class="form-group">
            <input type="text" id="find_keyword" name="find_keyword" class="form-control" placeholder="Nickname">
          </div>
          <button type="submit" class="btn btn-success">Search</button>
        <?php echo form_close(); ?>
        <ul class="nav navbar-nav navbar-right">
          <li><a href="<?php echo site_url(array('manager', 'execLogout')); ?>">Logout</a></li>
        </ul>
      </div>
    </div>
  </div>

  <div class="container">
