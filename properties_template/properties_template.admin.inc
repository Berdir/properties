<?php
// $Id$

/**
 * @file
 * Contains admin menu callbacks for properties_template.module.
 */

/**
 * Menu callback; Display a list of templates
 */
function properties_template_admin_templates_list() {
  $templates = properties_template_load_paging(20);

  $header = array(t('Name'), t('Label'), t('Operations'));
  $rows = array();
  $url_options = array('query' => drupal_get_destination());
  foreach ($templates as $template) {
    $links = array(l('edit', 'admin/config/content/properties/templates/edit/' . $template->name, $url_options), l('delete', 'admin/config/content/properties/templates/delete/' . $template->name, $url_options));

    $rows[] = array(
      $template->name,
      check_plain($template->label),
      implode(' ', $links),
    );
  }
  return theme('table', array('rows' => $rows, 'header' => $header, 'empty' => t('No templates found.')));
}