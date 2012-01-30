.. index::
   single: Test; Autenticazione HTTP

Come simulare un'autenticazione HTTP in un test funzionale
==========================================================

Se la propria applicazione necessita di autenticazione HTTP, passare il nome utente e
la password come variabili di ``createClient()``::

    $client = static::createClient(array(), array(
        'PHP_AUTH_USER' => 'nome_utente',
        'PHP_AUTH_PW'   => 'pa$$word',
    ));

Si possono anche sovrascrivere per ogni richiesta::

    $client->request('DELETE', '/post/12', array(), array(
        'PHP_AUTH_USER' => 'nome_utente',
        'PHP_AUTH_PW'   => 'pa$$word',
    ));
