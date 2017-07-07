<?php

namespace Drupal\adminic_tools;

use Drupal\Core\StringTranslation\StringTranslationTrait;

/**
 * Class Permissions
 *
 * @package Drupal\channels_efs
 */
class Permissions {

  use StringTranslationTrait;

  /**
   * Get permissions for Channels Blocks.
   *
   * @return array
   *   Permissions array.
   */
  public function getPermissions() {
    $permissions = [];
    $entity_discovery = \Drupal::service('adminic_tools.entity_discovery')->getManage();
    $blockCategories = array();
    foreach ($entity_discovery['content'] as $config_key => $config) {
      $blockCategories[] = $config_key;
    }
    foreach ($entity_discovery['configuration'] as $config_key => $config) {
      $blockCategories[] = $config_key;
    }

    foreach ($blockCategories as $category) {
      $permissions += [
        'adminic toolbar use ' . $category => [
          'title' => $this->t('Use %category', array('%category' => $category)),
        ],
      ];
    }

    return $permissions;
  }

}
