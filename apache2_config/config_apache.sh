#!/usr/bin/env sh

change_permission () {
	echo "Modifying permissions for ${3}/www"
	chown --verbose --recursive ${1}:${2} "${3}/www"
        chmod --verbose --recursive g+w "${3}/www"
}

cp_configs () {
	echo "Copying pigro.conf to ${2}/sites-available" &&
	cp --verbose "${1}/apache2_config/pigro.conf" "${2}/sites-available" &&

	echo "Copying pigro-cgi-bin.conf to ${2}/conf-available" &&
	cp --verbose "${1}/apache2_config/pigro.conf" "${2}/conf-available"
}

main () {

	local base_dir="/home/pi/PiGro"
	local apache_config="/etc/apache2"
	local user="pi"
	local group="www-data"

	if change_permission "${user}" "${group}" "${base_dir}"; then
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
