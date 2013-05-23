.. index::
    single: Helper di Console; Helper Progress
    
Helper Progress
===============

.. versionadded:: 2.2
    L'helper ``progress`` è stato aggiunto in Symfony 2.2.

Quando si eseguono comandi che devono girare a lungo, può essere utile mostrare una barra di progresso,
che si aggiorna durante l'esecuzione:

.. image:: /images/components/console/progress.png

Per mostrare dettagli sul progresso, usare la classe :class:`Symfony\\Component\\Console\\Helper\\ProgressHelper`,
passargli un numero totale di unità e avanzare il progresso man mano che il comando gira::

    $progress = $this->getHelperSet()->get('progress');

    $progress->start($output, 50);
    $i = 0;
    while ($i++ < 50) {
        // ... fare qualcosa

        // avanzare la barra di progresso di 1 unità
        $progress->advance();
    }

    $progress->finish();

Si può anche personalizzare il modo in cui il progresso viene mostrato, con vari
livelli di verbosità. Ciascuno di questi mostra diversi possibili
elementi, come la percentuale di completamento, una barra mobile, o informazioni
su attuale/totale (p.e. 10/50)::

    $progress->setFormat(ProgressHelper::FORMAT_QUIET);
    $progress->setFormat(ProgressHelper::FORMAT_NORMAL);
    $progress->setFormat(ProgressHelper::FORMAT_VERBOSE);
    $progress->setFormat(ProgressHelper::FORMAT_QUIET_NOMAX);
    // valore predefinito
    $progress->setFormat(ProgressHelper::FORMAT_NORMAL_NOMAX);
    $progress->setFormat(ProgressHelper::FORMAT_VERBOSE_NOMAX);

Si possono anche controllare i vari caratteri e la larghezza usata per
la barra::

    // la parte finale della barra
    $progress->setBarCharacter('<comment>=</comment>');
    // la parte in corso della barra
    $progress->setEmptyBarCharacter(' ');
    $progress->setProgressCharacter('|');
    $progress->setBarWidth(50);

Per tutte le opzioni disponibili, consultare la documentazione delle API di
:class:`Symfony\\Component\\Console\\Helper\\ProgressHelper`.

.. caution::

    Per questioni prestazionali, fare attenzione a non impostare un numero totale di passi
    troppo elevato. Per esempio, se si itera un gran numero
    di elementi, considerare un "passo" più piccolo, che aggiorni solamente ogni
    tot iterazioni::

        $progress->start($output, 500);
        $i = 0;
        while ($i++ < 50000) {
            // ... fare qualcosa

            // avanzare ogni 100 iterazioni
            if ($i % 100 == 0) {
                $progress->advance();
            }
        }