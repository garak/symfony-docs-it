.. index::
   pair: Forms; View

I form nei template
===================

Un :doc:`Form </guides/forms/overview>` di Symfony2 è fatto di campi. I campi
descrivono la forma semantica, non la loro rappresentazione per l'utente finale;
questo significa che un form non è legato all'HTML. Al contrario, è responsabilità
del grafico web la visualizzazione di ogni campo del form nel modo che vuole. Quindi
in Symfony2 la visualizzazione di un form in un template, può essere fatta
manualmente in modo semplice. Ma Symfony2 facilita l'integrazione e la personalizzazione
dei form, fornendo una raccolta di oggetti wrapper.

Visualizzare "manualmente" un form
----------------------------------

Prima di addentrarsi nei wrapper di Symfony2 e vedere come possono aiutare a visualizzare
i form, in modo facile, sicuro e veloce, bisogna sapere che niente di speciale accade sotto
il cofano. Per visualizzare un form di Symfony2, è possibile utilizzare un qualsiasi codice HTML:

.. code-block:: html

    <form action="#" method="post">
        <input type="text" name="name" />

        <input type="submit" />
    </form>

Se c'è un errore di validazione, bisognerebbe visualizzarlo e compilare i campi con
i valori inviati, in modo da rendere più facile risolvere velocemente i problemi. Basta
usare i metodi dedicati ai form:

.. code-block:: html+php

    <form action="#" method="post">
        <ul>
            <?php foreach ($form['name']->getErrors() as $error): ?>
                <li><?php echo $error[0] ?></li>
            <?php endforeach; ?>
        </ul>
        <input type="text" name="name" value="<?php $form['name']->getData() ?>" />

        <input type="submit" />
    </form>

I wrapper di Symfony2 aiutano a mantenere più corto il codice nei template,
rendono la visualizzazione del form facilmente personalizzabile, supportano
l'internazionalizzazione, la protezione CSRF, l'upload dei file e altro
out of the box. Le sezioni seguenti descrivono tutto quello che c'è da sapere.

Wrapper dei form nei template
-----------------------------

Per usufruire dei wrapper dei form in Symfony2, è necessario passare un oggetto speciale
al template, al posto dell'istanza dei form::

    // src/Application/HelloBundle/Controller/HelloController.php
    public function signupAction()
    {
        $form = ...;

        return $this->render('HelloBundle:Hello:signup.php', array(
            'form' => $this['templating.form']->get($form)
        ));
    }

Invece di passare l'istanza del form direttamente alla vista, la si avvolge con un
oggetto che fornisce i metodi che aiutano a visualizzare il modulo con maggiore
flessibilità (``$this['templating.form']->get($form)``).

Visualizzare un form
--------------------

Poiché la struttura globale di un form (il tag form, il pulsante di invio, ...)
non è definita dall'istanza del form, si è liberi di utilizzare il codice HTML che si desidera.
Un semplice template per un form, appare come segue:

.. code-block:: html

    <form action="#" method="post">
        <!-- Display the form fields -->

        <input type="submit" />
    </form>

Oltre alla struttura globale del form, è necessario un modo per visualizzare gli errori
a livello globale e i campi nascosti; questo è il lavoro rispettivamente dei metodi
``errors()``e ``hidden()``:

.. code-block:: html+php

    <form action="#" method="post">
        <?php echo $form->errors() ?>

        <!-- Display the form fields -->

        <?php echo $form->hidden() ?>

        <input type="submit" />
    </form>

.. note::
   Per impostazione predefinita, il metodo ``errors()`` genera una lista ``<ul>``, ma
   come si vedrà più avanti, può essere facilmente personalizzato.

Ultimo ma non meno importante, un form contenente un input di tipo file, deve avere
l'attributo ``enctype``; utilizzare il metodo ``form()`` per tenere conto di questo:

.. code-block:: html+php

    <?php echo $form->form('#') ?>

Visualizzare i campi
--------------------

Accedere ai campi di un form è facile, perché Symfony2 agisce sui form con la sintassi degli array:

.. code-block:: html+php

    <?php $form['title'] ?>

    <!-- accede ad un campo (first_name) nidificato in un gruppo (user) -->
    <?php $form['user']['first_name'] ?>

Essendo che ogni campo è una istanza di Field, non può essere visualizzato come mostrato sopra;
bisogna usare invece uno dei metodi wrapper.

Il metodo ``widget()`` visualizza una rappresentazione HTML di un campo:

.. code-block:: html+php

    <?php echo $form['title']->widget() ?>

.. note::
   Il widget dei campi è selezionato in base al nome della classe del campo (sotto
   ci sono maggiori informazioni).

Il metodo ``label()`` visualizza il tag ``<label>`` associato con il campo:

.. code-block:: html+php

    <?php echo $form['title']->label() ?>

Per impostazione predefinita, Symfony2 "umanizza" il nome del campo, ma si può anche
dare la propria etichetta:

.. code-block:: html+php

    <?php echo $form['title']->label('Dammi un titolo') ?>

.. note::
   Symfony2 internazionalizza automaticamente tutte le etichette e i messaggi di errore.

Il metodo ``errors()`` visualizza gli errori del campo:

.. code-block:: html+php

    <?php echo $form['title']->errors() ?>

È anche possibile ottenere i dati associati con il campo (i dati predefiniti o i
dati inviati dall'utente), attraverso il metodo ``data``:

.. code-block:: html+php

    <?php echo $form['title']->data() ?>

Definire la rappresentazione HTML
---------------------------------

I wrapper per i form si basano su template PHP per la visualizzazione HTML. Per
impostazione predefinita, Symfony2 viene fornito in bundle con i template per tutti
i campi predefiniti.

Ogni metodo wrapper è associato a un template PHP. Ad esempio, il
metodo ``errors()`` cerca un template ``errors.php``. Quello predefinito
è del tipo:

.. code-block:: html+php

    {# FrameworkBundle:Form:errors.php #}

    <?php if ($errors): ?>
        <ul>
            <?php foreach ($errors as $error): ?>
                <li><?php echo $view['translator']->trans($error[0], $error[1], 'validators') ?></li>
            <?php endforeach; ?>
        </ul>
    <?php endif; ?>

La tabella seguente mostra un elenco completo dei metodi e dei relativi template associati:

========== ==================
Metodo     Nome del template
========== ==================
``errors`` ``FrameworkBundle:Form:errors.php``
``hidden`` ``FrameworkBundle:Form:hidden.php``
``label``  ``FrameworkBundle:Form:label.php``
``render`` ``FrameworkBundle:Form:group/*/field_group.php`` o ``FrameworkBundle:Form:group/*/row.php`` (vedere sotto)
========== ==================

Il metodo ``widget()`` è un po' diverso in quanto sceglie il template per la
visualizzazione in base alla versione con sottolineatura del nome della classe
del campo. Per esempio, cerca per un template di ``input_field.php`` quando
deve visualizzare una istanza di ``InputField``:

.. code-block:: html+php

    <!-- FrameworkBundle:Form:widget/input_field.php -->
    <?php echo $generator->tag('input', $attributes) ?>

Se il template non esiste, il metodo cerca un template per una delle
classi genitrici del campo. Ecco perché non vi è alcun template predefinito
``password_field``, dal momento che la sua rappresentazione è esattamente la
stessa della sua classe padre (``input_field``).

Personalizzare la rappresentazione di un campo
----------------------------------------------

Il modo più semplice per personalizzare un campo è quello di passare attributi HTML
personalizzati come argomento del metodo ``widget()``:

.. code-block:: html+php

    <?php echo $form['title']->widget(array('class' => 'important')) ?>

Se si vuole sovrascrivere completamente la rappresentazione HTML di un widget, passare
un template PHP:

.. code-block:: html+php

    <?php echo $form['title']->widget(array(), 'HelloBundle:Form:input_field.php') ?>

Prototipazione
--------------

Quando si fa la prototipazione di un form, si può usare il metodo ``render()``
invece di visualizzare manualmente tutti i campi:

.. code-block:: html+php

    <?php echo $form->form('#') ?>
        <?php echo $form->render() ?>

        <input type="submit" />
    </form>

I wrapper per i campi hanno anche un metodo ``render()`` per visualizzare la "riga" di un campo:

.. code-block:: jinja

    <?php echo $form->form('#') ?>
        <?php echo $form->errors() ?>
        <table>
            <?php echo $form['first_name']->render() ?>
            <?php echo $form['last_name']->render() ?>
        </table>
        <?php echo $form->hidden() ?>
        <input type="submit" />
    </form>

Il metodo ``render()`` usa i template ``field_group.php`` e ``row.php``
per la visualizzazione:

.. code-block:: html+php

    <!-- FrameworkBundle:Form:group/table/field_group.php -->

    <?php echo $group->errors() ?>

    <table>
        <?php foreach ($group as $field): ?>
            <?php echo $field->render() ?>
        <?php endforeach; ?>
    </table>

    <?php echo $group->hidden() ?>

    <!-- FrameworkBundle:Form:group/table/row.php -->

    <tr>
        <th>
            <?php echo $field->label() ?>
        </th>
        <td>
            <?php echo $field->errors() ?>
            <?php echo $field->widget() ?>
        </td>
    </tr>

Così come ogni altro metodo, il metodo ``render()`` accetta un template come
argomento, per sovrascrivere la rappresentazione predefinita:

.. code-block:: html+php

    <?php echo $form->render('HelloBundle:Form:group/div/field_group.php') ?>

.. caution::
   Il metodo ``render()`` non è molto flessibile e dovrebbe essere usato solo
   per costruire prototipi.
