.. index::
    single: Sessioni, cookie

Evitare l'avvio di sessioni per utenti anonimi
==============================================

Le sessioni partono automaticamente ogni volta che si legge, scrive o anche solo si verifica
l'esistenza di dati in sessione. Questo vuole dire che potrebbe risultare difficile evitare di creare
un cookie di sessione per alcuni utenti: si deve evitare *qualsiasi*
accesso alla sessione.

Per esempio, un problema comune e quello di verificare i messaggi flash,
che sono in sessione. Il codice seguente garantisce che una sessione
parta *sempre*:

.. code-block:: html+jinja

    {% for flashMessage in app.session.flashbag.get('notice') %}
        <div class="flash-notice">
            {{ flashMessage }}
        </div>
    {% endfor %}

Anche se l'utente non è loggato e anche se non è stato creato alcun messaggio flash,
basta richiamare ``get()`` (o anche ``has()``) su ``flashbag`` per far partire
una sessione. Questo potrebbe influire negativamente sulle prestazioni, perché tutti gli
utenti riceverebbero un cookie di sessione. Per evitarli, aggiungere un controllo prima
di accedere a i messaggi flash:

.. code-block:: html+jinja

    {% if app.request.hasPreviousSession %}
        {% for flashMessage in app.session.flashbag.get('notice') %}
            <div class="flash-notice">
                {{ flashMessage }}
            </div>
        {% endfor %}
    {% endif %}
