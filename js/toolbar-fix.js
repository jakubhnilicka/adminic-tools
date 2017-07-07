/**
 * @file
 * A Backbone view for the toolbar element. Listens to mouse & touch.
 */

(function ($, Drupal) {

  'use strict';

    /**
     * Update the attributes of the toolbar bar element.
     */
    Drupal.toolbar.ToolbarVisualView.prototype.updateBarAttributes = function () {
    var isOriented = this.model.get('isOriented');
    this.$el.find('.toolbar-bar').removeAttr('data-offset-top');
    this.$el.toggleClass('toolbar-oriented', isOriented);
  }


}(jQuery, Drupal));
