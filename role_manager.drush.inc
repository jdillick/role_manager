<?php
/**
 * @file
 * role_manager.drush.inc
 */

use hfc\role_manager as rm;

/**
 * Implements hook_drush_command().
 */
function role_manager_drush_command() {
  return array(
    'revert-roles' => array(
      'description' => 'Revert roles to default permissions',
      'aliases' => array('rrol'),
    ),
  );
}

function drush_role_manager_revert_roles() {
  module_load_include('inc', 'role_manager', 'role_manager');
  drush_print(dt('Reverting roles to default permissions.'));
  rm\revert();
}
