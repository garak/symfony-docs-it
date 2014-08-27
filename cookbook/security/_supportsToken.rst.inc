Dopo che Symfony richiama ``createToken()``, chiamerà poi ``supportsToken()``
sulla classe (e su ogni altro ascoltatore di autenticazione) per cercare di capire chi deve
gestire il token. Questo è solo un modo per consentire l'uso di diversi meccanismi di autenticazione
per uno stesso firewall (in questo modo, si può per esempio provare prima
ad autenticare l'utente tramite un certificato o una chiave API ed eventualmente poi
usare un form di login).

Nella maggior parte dei casi, basta assicurarsi che questo metodo restituisca ``true`` per
un token che sia stato creato da ``createToken()``. La logica probabilmente sarà proprio
quella di questo esempio.
