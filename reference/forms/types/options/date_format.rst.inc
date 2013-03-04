format
~~~~~~

**tipo**: ``intero`` o ``stringa`` **predefinito**: ``IntlDateFormatter::MEDIUM``

Opzione passata alla classe ``IntlDateFormatter``, usata per trasformare il dato inserito
dall'utente nel formato appropriato. Questo è molto importante quando l'opzione `widget`_
è ``single_text`` e definisce il modo in cui il dato viene trasformato. Per impostazione
predefinita, il formato è determinato in base al locale dell'utente; si può sovrascriverlo
passando il formato come stringa.

Per maggiori informazioni sui formati validi, si veda `sintassi del formato Date/Time`_. Per
esempio, per rendere un singolo campo testuale, che si aspetta che l'utente inserisca ``yyyy-MM-dd``,
usare le seguenti opzioni::

    $builder->add('date_created', 'date', array(
        'widget' => 'single_text',
        'format' => 'yyyy-MM-dd',
    ));

.. _`sintassi del formato Date/Time`: http://userguide.icu-project.org/formatparse/datetime#TOC-Date-Time-Format-Syntax
