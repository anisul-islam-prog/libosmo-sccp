/* Osmocom STP (Signal Transfer Point) */

/* (C) 2015-2017 by Harald Welte <laforge@gnumonks.org>
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#include <osmocom/core/select.h>
#include <osmocom/core/utils.h>
#include <osmocom/core/logging.h>
#include <osmocom/core/application.h>
#include <osmocom/core/fsm.h>
#include <osmocom/vty/vty.h>
#include <osmocom/vty/command.h>
#include <osmocom/vty/ports.h>
#include <osmocom/vty/telnet_interface.h>
#include <osmocom/vty/logging.h>

#include <osmocom/sigtran/osmo_ss7.h>
#include <osmocom/sigtran/sccp_sap.h>
#include <osmocom/sigtran/sccp_helpers.h>
#include <osmocom/sigtran/protocol/sua.h>
#include <osmocom/sigtran/protocol/m3ua.h>

/* we only use logging sub-systems of the various libraries so far */
static const struct log_info_cat log_info_cat[] = {
};

static const struct log_info log_info = {
	.cat = log_info_cat,
	.num_cat = ARRAY_SIZE(log_info_cat),
};

static const char stp_copyright[] =
	"Copyright (C) 2015-2017 by Harald Welte <laforge@gnumonks.org>\r\n"
	"Contributions by Holger Freyther, Neels Hofmeyr\r\n"
	"License GPLv2+: GNU GPL Version 2 or later <http://gnu.org/licenses/gpl-2.0.html>\r\n"
	"This is free software: you are free ot change and redistribute it.\r\n"
	"There is NO WARRANTY, to the extent permitted by law.\r\n\r\n"
	"Free Software lives by contribution.  If you use this, please contribute!\r\n";

static struct vty_app_info vty_info = {
	.name	= "osmo-stp",
	.copyright = stp_copyright,
	.version = PACKAGE_VERSION,
	.go_parent_cb = osmo_ss7_vty_go_parent,
	.is_config_node = osmo_ss7_is_config_node,
};

static struct {
	bool daemonize;
	const char *config_file;
} cmdline_config = {
	.daemonize = false,
	.config_file = "osmo-stp.cfg",
};

static void print_help(void)
{
	printf("  -h --help			This text.\n");
	printf("  -D --daemonize		Fork teh process into a background daemon\n");
	printf("  -c --config-file filename	The config file to use. Default: ./osmo-stp.cfg\n");
	printf("  -V --version			Print the version of OsmoSTP\n");
}

static void handle_options(int argc, char **argv)
{
	while (1) {
		int option_index = 0, c;
		static const struct option long_options[] = {
			{ "help", 0, 0, 'h' },
			{ "daemonize", 0, 0, 'D' },
			{ "config-file", 1, 0, 'c' },
			{ "version", 0, 0, 'V' },
			{ NULL, 0, 0, 0 }
		};

		c = getopt_long(argc, argv, "hDc:V", long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 'h':
			print_help();
			exit(0);
			break;
		case 'D':
			cmdline_config.daemonize = true;
			break;
		case 'c':
			cmdline_config.config_file = optarg;
			break;
		case 'V':
			print_version(1);
			exit(0);
			break;
		default:
			fprintf(stderr, "Error in command line options. Exiting\n");
			exit(1);
			break;
		}
	}
}

int main(int argc, char **argv)
{
	int rc;

	osmo_init_logging(&log_info);
	vty_init(&vty_info);

	handle_options(argc, argv);

	fputs(stp_copyright, stdout);
	fputs("\n", stdout);

	osmo_ss7_init();
	osmo_fsm_log_addr(false);
	logging_vty_add_cmds(&log_info);
	osmo_ss7_vty_init_sg();

	rc = vty_read_config_file(cmdline_config.config_file, NULL);
	if (rc < 0) {
		fprintf(stderr, "Failed to parse the config file '%s'\n",
			cmdline_config.config_file);
		exit(1);
	}

	rc = telnet_init_dynif(NULL, NULL, vty_get_bind_addr(), OSMO_VTY_PORT_STP);
	if (rc < 0) {
		perror("Erro binding VTY port\n");
		exit(1);
	}

	if (cmdline_config.daemonize) {
		rc = osmo_daemonize();
		if (rc < 0) {
			perror("Error during daemonize");
			exit(1);
		}
	}

	while (1) {
		rc = osmo_select_main(0);
		if (rc < 0)
			exit(3);
	}
}