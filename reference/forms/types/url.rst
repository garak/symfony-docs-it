.. index::
   single: Form; Campi; url

Tipo di campo url
=================

Il campo ``url`` è un campo testuale, che aggiunge al valore inserito un prefisso con
un protocollo dato (p.e. ``http://``), se il valore inserito non ha già tale
protocollo.

+---------------+-------------------------------------------------------------------+
| Reso come     | campo ``input url``                                               |
+---------------+-------------------------------------------------------------------+
| Opzioni       | - `default_protocol`_                                             |
+---------------+-------------------------------------------------------------------+
| Opzioni       | - `data`_                                                         |
| ereditate     | - `disabled`_                                                     |
|               | - `empty_data`_                                                   |
|               | - `error_bubbling`_                                               |
|               | - `error_mapping`_                                                |
|               | - `label`_                                                        |
|               | - `label_attr`_                                                   |
|               | - `mapped`_                                                       |
|               | - `max_length`_                                                   |
|               | - `read_only`_                                                    |
|               | - `required`_                                                     |
|               | - `trim`_                                                         |
+---------------+-------------------------------------------------------------------+
| Tipo genitore | :doc:`text </reference/forms/types/text>`                         |
+---------------+-------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\UrlType` |
+---------------+-------------------------------------------------------------------+

Opzioni del campo
-----------------

default_protocol
~~~~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``http``

Se un valore inserito non inizia per nessun protocollo (p.e. ``http://``,
``ftp://``, ecc.), questo protocollo sarà aggiunto come prefisso alla stringa, quando
i dati saranno inviati al form.

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`form </reference/forms/types/form>`:

.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/empty_data.rst.inc
    :end-before: DEFAULT_PLACEHOLDER

Il valore predefinito è ``''`` (stringa vuota).

.. include:: /reference/forms/types/options/empty_data.rst.inc
    :start-after: DEFAULT_PLACEHOLDER

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc
