.. index::
   single: Console; Inviare email
   single: Console; Generare URL

Generare URL e inviare email da console
=======================================

Sfortunatamente, la linea di comando non sa nulla degli host virtuali o dei
nomi di dominio. Questo vuol dire che, se si generano URL assoluti da un comando
da console, probabilmente si otterrà qualcosa come ``http://localhost/pippo/pluto``,
il che non è molto utile.

Per risolvere il problema, bisogna configrare il "contesto della richiesta", che un modo
particolare per dire che occorre configurare il proprio ambiente, in modo tale che sappia
quale URL va usata per la generazione.

Ci sono due modi per configurare il contesto della richiesta: a livello di applicazione
o per comando.

Configurare il contesto della richiesta per comando
---------------------------------------------------

Per cambiare un singolo comando, si può semplicemente recuperare il servizio del contesto
della richiesta e sovrascrivere le sue impostazioni::

   // src/Acme/DemoBundle/Command/DemoCommand.php

   // ...
   class DemoCommand extends ContainerAwareCommand
   {
       protected function execute(InputInterface $input, OutputInterface $output)
       {
           $context = $this->getContainer()->get('router')->getContext();
           $context->setHost('example.com');
           $context->setScheme('https');

           // ... il proprio codice
       }
   }

Usare lo spool memory
---------------------

L'invio di email in un comando da console funziona nello stesso modo descritto in
:doc:`/cookbook/email/email`, tranne per il fatto che viene usato lo spool memory.

Quando si usa lo spool memory (vedere :doc:`/cookbook/email/spool` per maggiori
informazioni), bisogna essere consapevoli che, a causa del modo in cui Symfony gestisce i comandi da
console, le email non sono inviate automaticamente. Ci si deve occupare del flush
della coda da soli. Usare il codice seguente per inviare email da dentro un
comando di console::

    $container = $this->getContainer();
    $mailer = $container->get('mailer');
    $spool = $mailer->getTransport()->getSpool();
    $transport = $container->get('swiftmailer.transport.real');

    $spool->flushQueue($transport);
    
Un'altra possibilità è quella di creare un ambiente usato solo dai comandi
di console e usare un metodo di spool differente. 
    
.. note::

    Ci si deve occupare dello spool solo quando si usa lo spool memory. 
    Se invece si usa lo spool file (o nessuno spool), non occorre alcun
    flush manuale della coda all'interno del comando.