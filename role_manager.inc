<?php
/**
 * @file
 * role_manager.inc
 */

namespace hfc\role_manager;
use hfc\perms_utils as perms;
use \stdClass;

if ( ! defined('PERMS_UTILS_PATH') ) {
  define('PERMS_UTILS_PATH', DRUPAL_ROOT . '/sites/all/libraries/perms_utils/perms_utils.php');
}

require_once PERMS_UTILS_PATH;

function revert() {
  create_roles();
  set_permissions();
}

function create_roles() {
  $roles_names = array_map('strtolower', roles());
  $existing_roles = array_map('strtolower', user_roles());

  foreach ( $roles_names as $role_name ) {
    // only create roles that don't already exist
    if ( ! in_array($role_name, $existing_roles) ) {
      $new_role = new stdClass;
      $new_role->name = $role_name;
      user_role_save($new_role);
    }
  }
}

function roles() {
  return array_keys(permissions());
}

function set_permissions() {
  $all_existing_perms = array_keys(user_permission_get_modules());
  foreach (permissions() as $role_name => $permissions) {
    $role_id = user_role_load_by_name($role_name)->rid;
    $role_perms = perms\filter_inactive_permissions($permissions);

    user_role_revoke_permissions($role_id, $all_existing_perms);
    if ( $role_perms ) {
      user_role_grant_permissions($role_id, $role_perms);
    }
  }
  // Administrator gets everything
  user_role_grant_permissions(user_role_load_by_name('administrator')->rid,
    $all_existing_perms);
}

function permissions() {
  $permissions = &drupal_static(__FUNCTION__);
  if ( ! isset($permissions) ) {
    $permissions = module_invoke_all('roles_info');
    drupal_alter('roles_info', $permissions);
    $permissions = process_perms($permissions);
  }

  return $permissions;
}

function process_perms($perms) {
  $permissions = array();
  foreach ( $perms as $role => $permission_set ) {
    $role_perms = array();
    foreach ( $permission_set as $action => $level ) {
      $permission_function = 'hfc\\perms_utils\\' . $action;
      if ( function_exists($permission_function) ) {
        $role_perms = array_merge($role_perms, $permission_function($level));
      }
    }
    $permissions[$role] = $role_perms;
  }

  return $permissions;
}
