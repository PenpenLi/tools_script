<script>
$(function() {
    $('#modal-login').modal({backdrop:'static', show:true});

    $('#button-signup').click(function() {
        $('#modal-login').modal('hide');
        $('#modal-signup').modal({backdrop:'static', show:true});
    });

    $('#button-login').click(function() {
        $('#modal-login').modal({backdrop:'static', show:true});
        $('#modal-signup').modal('hide');
    });
});
</script>

<div class="modal fade" id="modal-login">
  <div class="modal-dialog">
    <div class="modal-content">
      <div class="modal-header">
        <h4 class="modal-title">One Punch Tool Login</h4>
      </div>
    <form method="post" action="<?php echo site_url(array('manager', 'execLogin')); ?>" class="form-horizontal" role="form">
      <div class="modal-body">
          <div class="form-group">
            <label for="form-email" class="col-sm-2 control-label">Email</label>
            <div class="col-sm-10">
              <input type="email" class="form-control" id="form-email" name="email" placeholder="Email" required />
            </div>
          </div>
          <div class="form-group">
            <label for="form-password" class="col-sm-2 control-label">Password</label>
            <div class="col-sm-10">
              <input type="password" class="form-control" id="form-password" name="password" placeholder="Password" required />
            </div>
          </div>
      </div>
      <div class="modal-footer">
          <button type="button" class="btn btn-warning" id="button-signup">Sign up</button> <button type="submit" class="btn btn-primary">Login</button>
      </div>
    </form>
    </div>
  </div>
</div>

<div class="modal fade" id="modal-signup">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <h4 class="modal-title">Tool Sign Up</h4>
            </div>
            <form method="post" action="<?php echo site_url(array('manager', 'execSignUp')); ?>" class="form-horizontal" role="form">
                <div class="modal-body">
                    <div class="form-group">
                        <label for="form-email" class="col-sm-4 control-label">Email</label>
                        <div class="col-sm-8">
                            <input type="email" class="form-control" id="form-email" name="email" placeholder="Email" required />
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="form-password" class="col-sm-4 control-label">Password</label>
                        <div class="col-sm-8">
                            <input type="password" class="form-control" id="form-password" name="password" placeholder="Password" required />
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="form-confirm-password" class="col-sm-4 control-label">Confirm Password</label>
                        <div class="col-sm-8">
                            <input type="password" class="form-control" id="form-confirm-password" name="confirm_password" placeholder="Confirm Password" required />
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="form-name" class="col-sm-4 control-label">Name</label>
                        <div class="col-sm-8">
                            <input type="text" class="form-control" id="form-name" name="name" placeholder="Name" required />
                        </div>
                    </div>
                </div>
                <div class="modal-footer">
                    <button type="button" class="btn btn-default" id="button-login">Login</button> <button type="submit" class="btn btn-primary">Sign up</button>
                </div>
            </form>
        </div>
    </div>
</div>
