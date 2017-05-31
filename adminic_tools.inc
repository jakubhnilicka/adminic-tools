<?php

/**
 * @file
 * Adminic Tools helper functions.
 */

use Drupal\Core\Cache\Cache;
use Drupal\Core\Url;

function add_manage($type, $add_url, $list_url, $list_parametter = NULL, $crete_permissions = NULL, $add_bundle_url = NULL, $add_bundle_label = NULL) {
  $user = \Drupal::currentUser();
  $entity = \Drupal::entityTypeManager()->getDefinition($type);
  $admin_permissions = $entity->getAdminPermission();
  $link_templates = $entity->getLinkTemplates();

  if (is_null($list_parametter)) {
    $list_parametter = $type;
  }
  if (in_array('collection', $link_templates)) {
    $collection_url = $link_templates['collection'];
  }
  else {
    $collection_url = NULL;
  }

  $links = [];

  if ($user->isAuthenticated()) {
    $links_cache_contexts[] = 'user';

    $variantions = \Drupal::entityTypeManager()->getStorage($type)->loadMultiple();

    foreach ($variantions as $variantion) {

      $bundle = $variantion->id();
      $name = $variantion->label();

      $links[$bundle] = [];
      $links[$bundle]['name'] = $name;
      $links[$bundle]['bundle'] = $bundle;
      $links[$bundle]['attributes'] = [
        'class' => ['content-links', 'content-' . $type . '-' . $bundle],
      ];

      // Add Route
      if ($crete_permissions != NULL) {
        $add_permissions = str_replace('{bundle}', $bundle, $crete_permissions);
      }
      if ($crete_permissions == NULL || (isset($add_permissions) && $user->hasPermission($add_permissions))) {
        if (!is_null($add_url)) {
          $links[$bundle]['add_url'] = Url::fromRoute($add_url, [$type => $bundle])
            ->toString();
        }
      }

      // Edit route
      if ($user->hasPermission($admin_permissions)) {
        $links[$bundle]['edit_url'] = Url::fromRoute(
          $variantion->toUrl()->getRouteName(),
          $variantion->toUrl()->getRouteParameters())
          ->toString();
      }

      // List route
      if (!is_null($list_url)) {
        $links[$bundle]['list_url'] = Url::fromRoute($list_url, [$list_parametter => $bundle])
          ->toString();
      }

    }

    $return = [];
    $return['#theme'] = 'adminic_content_bundler';

    if ($user->hasPermission($admin_permissions)) {

      if (!is_null($collection_url)) {
        $return['#collection_url'] = $collection_url;
      }

      if (is_null($add_bundle_url) && array_key_exists('add-form', $link_templates)) {
        $add_bundle_url = $link_templates['add-form'];
        $return['#add_bundle_url'] = $add_bundle_url;
      }
      elseif (!is_null($add_bundle_url)) {
        $return['#add_bundle_url'] = Url::fromRoute($add_bundle_url)
          ->toString();
      }

      if (is_null($add_bundle_label)) {
        $add_bundle_label = 'Add ' . $entity->getLabel();
      }
      $return['#add_bundle_text'] = $add_bundle_label;
    }
    $return['#links'] = $links;
    $return['#title'] = $entity->getLabel();
    $return['#attributes'] = [
      'class' => ['toolbar-menu', $type . '-add'],
    ];
    $return['#attributes'] = [
      '#cache' => [
        'contexts' => Cache::mergeContexts(['user.roles:authenticated'], $links_cache_contexts),
      ],
    ];

    return $return;
  }

  return NULL;
}

function _adminic_tools_entity_discovery() {
  /** @var \Drupal\Core\Url $valid_url */
  $entities = \Drupal::entityTypeManager()->getDefinitions();
  $path_validator = \Drupal::service('path.validator');
  $entity_info = [];
  // Content Entities
  foreach ($entities as $entity) {
    /** @var \Drupal\Core\Entity\ContentEntityType $id */
    $id = $entity->id();
    $group = $entity->getGroup();
    if ($group == 'content') {
      $bundle_entity_type = $entity->getBundleEntityType();
      if ($bundle_entity_type != NULL) {
        $entity_bundle = \Drupal::entityTypeManager()
          ->getDefinition($bundle_entity_type);
        $entity_info[$group][$id]['type'] = $entity_bundle->id();
        $entity_info[$group][$id]['list_url'] = NULL;
        $entity_info[$group][$id]['add_bundle_url'] = NULL;

        $links = array_keys($entity_bundle->getLinkTemplates());

        if (in_array('add-form', $links)) {
          $add_form_url = $entity_bundle->getLinkTemplate('add-form');
          $valid_url = $path_validator->getUrlIfValid($add_form_url);
          if ($valid_url != FALSE) {
            $entity_info[$group][$id]['add_bundle_url'] = $valid_url->getRouteName();
          }
        }

        $entity_info[$group][$id]['add_bundle_label'] = 'New ' . $entity->getLabel() . ' Type';
        if (in_array('collection', $links)) {
          $add_form_url = $entity_bundle->getLinkTemplate('collection');
          $valid_url = $path_validator->getUrlIfValid($add_form_url);
          if ($valid_url != FALSE) {
            $entity_info[$group][$id]['list_url'] = $valid_url->getRouteName();
          }
        }
        $entity_info[$group][$id]['list_parametter'] = $entity->id();
        $entity_info[$group][$id]['add_url'] = NULL;
        $entity_info[$group][$id]['crete_permissions'] = $entity_bundle->getAdminPermission();
        unset($entities[$entity_bundle->id()]);
      }
    }
  }

  $config_entities = [
    'block',
    'image_style',
    'menu',
    'view',
    'user_role',
    'menu_link_content',
    'filter_format',
    'date_format',
  ];
  \Drupal::moduleHandler()->alter('entity_disovery_config', $config_entities);
  // Config entities
  foreach ($entities as $entity) {
    /** @var \Drupal\Core\Config\Entity\ConfigEntityType $entity */
    $id = $entity->id();
    $group = $entity->getGroup();
    if ($group == 'configuration' && in_array($id, $config_entities)) {
      $entity_info[$group][$id]['type'] = $entity->id();
      $entity_info[$group][$id]['list_url'] = NULL;
      $entity_info[$group][$id]['add_bundle_url'] = NULL;
      $links = array_keys($entity->getLinkTemplates());

      if (in_array('add-form', $links)) {
        $add_form_url = $entity->getLinkTemplate('add-form');
        $valid_url = $path_validator->getUrlIfValid($add_form_url);
        if ($valid_url != FALSE) {
          $entity_info[$group][$id]['add_bundle_url'] = $valid_url->getRouteName();
        }
      }

      $entity_info[$group][$id]['add_bundle_label'] = 'Add ' . $entity->getLabel() . ' Type';
      if (in_array('collection', $links)) {
        $add_form_url = $entity->getLinkTemplate('collection');
        $valid_url = $path_validator->getUrlIfValid($add_form_url);
        if ($valid_url != FALSE) {
          $entity_info[$group][$id]['list_url'] = $valid_url->getRouteName();
        }
      }
      $entity_info[$group][$id]['list_parametter'] = $entity->id();
      $entity_info[$group][$id]['add_url'] = NULL;
      $entity_info[$group][$id]['crete_permissions'] = $entity->getAdminPermission();
      unset($entities[$entity->id()]);
    }
  }

  \Drupal::moduleHandler()->alter('entity_disovery', $entity_info);

  return $entity_info;
}
