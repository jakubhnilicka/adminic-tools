<?php

namespace Drupal\adminic_tools\Controller;

use Drupal\Core\Controller\ControllerBase;

/**
 * Defines a class to build a listing of custom block entities.
 *
 * @see \Drupal\chatbot_efs\Entity\Digest
 */
class Config extends ControllerBase {

  /**
   * {@inheritdoc}
   *
   * We override ::render() so that we can add our own content above the table.
   * parent::render() is where EntityListBuilder creates the table using our
   * buildHeader() and buildRow() implementations.
   */
  public function content() {
    $entity_discovery = \Drupal::service('adminic_tools.entity_discovery');
    $entity_discovery->setTheme('adminic_content_page');
    $manage = $entity_discovery->getManage();
    $build = $manage['configuration'];
    $build['#prefix'] = '<div class="config-settings row">';
    $build['#suffix'] = '</div>';

    return $build;
  }

}
