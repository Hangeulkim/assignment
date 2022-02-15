all:
	make ssu_rsync
	make ssu_crontab
	make ssu_crond

ssu_rsync:
	gcc -o ssu_rsync ssu_rsync.c

ssu_crontab:
	gcc -o ssu_crontab ssu_crontab.c

ssu_crond:
	gcc -o ssu_crond ssu_crond.c -lpthread

clean :
	rm ssu_crond ssu_crontab ssu_rsync
