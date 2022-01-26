#!/usr/bin/env sh

change_permission () {
	echo "Modifying permissions for ${base_dir}/www"
	chown -vR ${1}:${2} "${base_dir}/www"
        chmod -vR g+w "${base_dir}/www"
}

cp_configs () {
	echo "Copying pigro.conf to ${2}/sites-available" &&
	cp -v "${1}/apache2_config/pigro.conf" "${2}/sites-available" &&

	echo "Copying pigro-cgi-bin.conf to ${2}/conf-available" &&
	cp -v "${1}/apache2_config/pigro.conf" "${2}/conf-available"
}

main () {

	base_dir="/home/pi/PiGro"
	apache_config="/etc/apache2"
	user="pi"
	group="www-data"

	if change_permission "${user}" "${group}"; then
		echo "Success"
	else
		echo "Failed"
		exit 1
	fi

	if cp_configs "${base_dir}" "${apache_config}"; then
		echo "Success"
	else
		echo "Failed"
		exit 1
	fi

	echo "Enabling PiGro site in Apache2"
	a2ensite pigro

	echo "Disabling default page"
	a2dissite 000-default


	echo "Restarting Apache2"
	systemctl restart apache2

}

# Check for root permissions
if [ $(id -u) -ne 0 ]
then
	echo "Escalating to root"
	sudo ./"$0"
else
	echo "Running as root"
	main
fi
