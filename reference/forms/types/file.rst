.. index::
   single: Form; Campi; file

Tipo di campo file
==================

Il tipo ``file`` rappresenta un input per caricare file.

+---------------+---------------------------------------------------------------------+
| Reso come     | campo ``input`` ``file``                                            |
+---------------+---------------------------------------------------------------------+
| Opzioni       | - `required`_                                                       |
| ereditate     | - `label`_                                                          |
|               | - `read_only`_                                                      |
|               | - `error_bubbling`_                                                 |
+---------------+---------------------------------------------------------------------+
| Tipo genitore | :doc:`form</reference/forms/types/field>`                           |
+---------------+---------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\FileType`  |
+---------------+---------------------------------------------------------------------+

Utilizzo di base
----------------

Si supponga di avere in un form:

.. code-block:: php

    $builder->add('attachment', 'file');

.. caution::

    Non dimenticare di aggiungere l'attributo ``enctype`` nel tag form:
    ``<form action="#" method="post" {{ form_enctype(form) }}>``.

Quando il form viene inviato, il campo ``attachment`` sarà un'istanza di
:class:`Symfony\\Component\\HttpFoundation\\File\\UploadedFile`. Può essere
usata per spotare il file ``attachment`` in una posizione permanente:

.. code-block:: php

    use Symfony\Component\HttpFoundation\File\UploadedFile;

    public function uploadAction()
    {
        // ...

        if ($form->isValid()) {
            $someNewFilename = ...
        
            $form['attachment']->getData()->move($dir, $someNewFilename);

            // ...
        }

        // ...
    }

Il metodo ``move()`` accetta come parametri una cartella e un nome di file.
Si può calcolare il nome del file in uno dei modi seguenti::

    // usare il nome del file originale
    $file->move($dir, $file->getClientOriginalName());

    // calcolare un nome casuale e provare a indovinare l'estensione (più sicuro)
    $extension = $file->guessExtension();
    if (!$extension) {
        // l'estensione non può essere indovinata
        $extension = 'bin';
    }
    $file->move($dir, rand(1, 99999).'.'.$extension);

L'uso del nome originale, tramite ``getClientOriginalName()``, non è sicuro, perché
potrebbe essere stato manipolato dall'utente. Inoltre, può contenere caratteri
che non sono consentiti nei nomi di file. Si dovrebbe ripulire il nome prima
di utilizzarlo direttamente.

Leggere il :doc:`ricettario </cookbook/doctrine/file_uploads>` per un esempio di
come gestire un caricamento di file associato con un'entità Doctrine.

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`field</reference/forms/types/field>`:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc