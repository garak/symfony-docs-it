.. index::
   single: Dependency Injection; Passi di compilatore
   single: Contenitore di servizi; Passi di compilatore

Come lavorare con i passi di compilatore nei bundle
===================================================

I passi di compilatore danno l'opportunità di manipolare definizioni di altri servizi,
che sono stai registrati nel contenitore di servizi. Si può leggere come crearli nella
sezione dei componenti ":doc:`/components/dependency_injection/compilation`".
Per registrare un passo di compilatore da un bundle, occorre aggiungerlo al metodo di
costruzione della classe di definizione del bundle::

    // src/Acme/MailerBundle/AcmeMailerBundle.php
    namespace Acme\MailerBundle;

    use Symfony\Component\HttpKernel\Bundle\Bundle;
    use Symfony\Component\DependencyInjection\ContainerBuilder;

    use Acme\MailerBundle\DependencyInjection\Compiler\CustomCompilerPass;

    class AcmeMailerBundle extends Bundle
    {
        public function build(ContainerBuilder $container)
        {
            parent::build($container);

            $container->addCompilerPass(new CustomCompilerPass());
        }
    }

Uno dei più comuni casi d'uso dei passi di compilatore è lavorare con i servizi con tag
(leggere di più sui tag nella sezione dei componenti ":doc:`/components/dependency_injection/tags`").
Se si usando tag personalizzati nei bundle, i nomi di tag sono composti dal nome
del bundle (in minuscolo, con trattini bassi come separatori), seguito da un
punto e quindi dal nome "reale". Per esempio, se si vuole introdurre una sorta di tag
"trasporto" in un AcmeMailerBundle, lo si dovrebbe chiamare
``acme_mailer.transport``.
