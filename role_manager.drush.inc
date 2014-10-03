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
    'role-check' => array(
      'description' => 'Verify roles are at default permissions',
      'aliases' => array('crol'),
    ),
  );
}

function drush_role_manager_revert_roles() {
  module_load_include('inc', 'role_manager', 'role_manager');
  drush_print(dt('Reverting roles to default permissions.'));
  rm\revert();
}

function drush_role_manager_role_check() {
  module_load_include('inc', 'role_manager', 'role_manager');
  drush_print(dt('Checking roles match default permissions.'));
  $differences = rm\check();

  foreach ( $differences as $role => $difference ) {
    if ( ! $difference ) {
      drupal_set_message(t('@r looks GOOD.', array('@r' => $role)), 'ok');
      continue;
    }
    drupal_set_message(t('@r looks BAD.', array('@r' => $role)), 'error', TRUE);

    foreach ( $difference as $perm => $status ) {
      drupal_set_message(t("  @p is @s!", array(
        '@p' => $perm,
        '@s' => ( $status ? "ENABLED" : "DISABLED" ),
      )), 'error', TRUE);
    }
  }
}
