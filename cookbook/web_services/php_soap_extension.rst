.. index::
    single: Servizi web; SOAP

.. _how-to-create-a-soap-web-service-in-a-symfony2-controller:

Creare un servizio web SOAP in un controllore di Symfony
========================================================

Impostare un controllore per agire da server SOAP è semplice, con un paio
di strumenti. Occorre avere, ovviamente, l'estensione `PHP SOAP`_ installata.  
Poiché l'estensione PHP SOAP non può attualmente generare un WSDL, se ne deve
creare uno da zero, oppure usare un generatore di terze parti.

.. note::

    Ci sono molte implementazioni di server SOAP disponibili per PHP.
    `Zend SOAP`_ e `NuSOAP`_ sono due esempi. Anche se useremo
    l'estensione PHP SOAP nei nostri esempi, l'idea generale dovrebbe essere
    applicabile ad altre implementazioni.

SOAP funziona espondendo i metodi di un oggetto PHP a un'entità esterna
(alla persona che usa il servizio SOAP). Per iniziare, creare una classe
``HelloService``, che rappresenta la funzionalità che sarà esposta nel
servizio SOAP. In questo caso, il servizio SOAP consentirà al client di richiamare
un metodo chiamto ``hello``,  che invia un'email::

    // src/Acme/SoapBundle/Services/HelloService.php
    namespace Acme\SoapBundle\Services;

    class HelloService
    {
        private $mailer;

        public function __construct(\Swift_Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        public function hello($name)
        {

            $message = \Swift_Message::newInstance()
                                    ->setTo('me@example.com')
                                    ->setSubject('Servizio Hello')
                                    ->setBody($name . ' dice ciao!');

            $this->mailer->send($message);

            return 'Hello, ' . $name;
        }
    }

Quindi, si può dire a Symfony di creare un'istanza di questa classe.
Poiché la classe invia un'email, è stata concepita per accettare un'istanza di
``Swift_Mailer``. Usando il contenitore di servizi, possiamo configurare Symfony
per costruire un oggetto ``HelloService`` in modo appropriato:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            hello_service:
                class: Acme\SoapBundle\Services\HelloService
                arguments: ["@mailer"]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <services>
            <service id="hello_service" class="Acme\SoapBundle\Services\HelloService">
                <argument type="service" id="mailer"/>
            </service>
        </services>

    .. code-block:: php

        // app/config/config.php
        $container
            ->register('hello_service', 'Acme\SoapBundle\Services\HelloService')
            ->addArgument(new Reference('mailer'));

Di seguito un esempio di un controllore che è in grado di gestire una richiesta
SOAP. Se ``indexAction()`` è accessibile tramite la rotta ``/soap``, il documento
WSDL può essere recuperato tramite ``/soap?wsdl``.

.. code-block:: php

    namespace Acme\SoapBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Symfony\Component\HttpFoundation\Response;

    class HelloServiceController extends Controller
    {
        public function indexAction()
        {
            $server = new \SoapServer('/percorso/di/hello.wsdl');
            $server->setObject($this->get('hello_service'));

            $response = new Response();
            $response->headers->set('Content-Type', 'text/xml; charset=ISO-8859-1');

            ob_start();
            $server->handle();
            $response->setContent(ob_get_clean());

            return $response;
        }
    }

Si notino le chiamate a ``ob_start()`` e ``ob_get_clean()``. Qesti metodi
controllano il `buffer dell'output`_, che consente di "intrappolare" l'output
inviato da ``$server->handle()``. Questo si rende necessario, in quanto Symfony
si aspetta che il controllore restituisca un oggetto ``Response``, con l'output
come contenuto. Si deve anche ricordare di impostare l'header "Content-Type" a
"text/xml", che è quello che il client si aspetta. Quindi, si usa ``ob_start()`` 
per iniziare il buffer di STDOUT e  ``ob_get_clean()`` per inviare l'output
nel contenuto della risposta e per pulire il buffer. Infine, è tutto pronto
per restituire l'oggetto ``Response``.

Di seguito un esempio che richiama il servizio, usando un client `NuSOAP`_. Questo esempio
presume che ``indexAction`` nel controllore visto sopra sia accessibile tramite la rotta
``/soap``::

    $client = new \Soapclient('http://example.com/app.php/soap?wsdl', true);

    $result = $client->call('hello', array('name' => 'Scott'));

Di seguito, un esempio di WSDL

.. code-block:: xml

    <?xml version="1.0" encoding="ISO-8859-1"?>
    <definitions xmlns:SOAP-ENV="http://schemas.xmlsoap.org/soap/envelope/"
        xmlns:xsd="http://www.w3.org/2001/XMLSchema"
        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
        xmlns:SOAP-ENC="http://schemas.xmlsoap.org/soap/encoding/"
        xmlns:tns="urn:arnleadservicewsdl"
        xmlns:soap="http://schemas.xmlsoap.org/wsdl/soap/"
        xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/"
        xmlns="http://schemas.xmlsoap.org/wsdl/"
        targetNamespace="urn:helloservicewsdl">

        <types>
            <xsd:schema targetNamespace="urn:hellowsdl">
                <xsd:import namespace="http://schemas.xmlsoap.org/soap/encoding/" />
                <xsd:import namespace="http://schemas.xmlsoap.org/wsdl/" />
            </xsd:schema>
        </types>

        <message name="helloRequest">
            <part name="name" type="xsd:string" />
        </message>

        <message name="helloResponse">
            <part name="return" type="xsd:string" />
        </message>

        <portType name="hellowsdlPortType">
            <operation name="hello">
                <documentation>Hello World</documentation>
                <input message="tns:helloRequest"/>
                <output message="tns:helloResponse"/>
            </operation>
        </portType>

        <binding name="hellowsdlBinding" type="tns:hellowsdlPortType">
            <soap:binding style="rpc" transport="http://schemas.xmlsoap.org/soap/http"/>
            <operation name="hello">
                <soap:operation soapAction="urn:arnleadservicewsdl#hello" style="rpc"/>

                <input>
                    <soap:body use="encoded" namespace="urn:hellowsdl"
                        encodingStyle="http://schemas.xmlsoap.org/soap/encoding/"/>
                </input>

                <output>
                    <soap:body use="encoded" namespace="urn:hellowsdl"
                        encodingStyle="http://schemas.xmlsoap.org/soap/encoding/"/>
                </output>
            </operation>
        </binding>

        <service name="hellowsdl">
            <port name="hellowsdlPort" binding="tns:hellowsdlBinding">
                <soap:address location="http://example.com/app.php/soap" />
            </port>
        </service>
    </definitions>

.. _`PHP SOAP`:            http://php.net/manual/it/book.soap.php
.. _`NuSOAP`:              http://sourceforge.net/projects/nusoap
.. _`buffer dell'output`:  http://php.net/manual/it/book.outcontrol.php
.. _`Zend SOAP`:           http://framework.zend.com/manual/en/zend.soap.server.html
