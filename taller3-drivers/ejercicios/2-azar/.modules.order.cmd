cmd_/vagrant/ejercicios/2-azar/modules.order := {   echo /vagrant/ejercicios/2-azar/azar.ko; :; } | awk '!x[$$0]++' - > /vagrant/ejercicios/2-azar/modules.order
