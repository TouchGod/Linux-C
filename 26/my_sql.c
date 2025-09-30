#include <header.h>
#include <mysql/mysql.h>

int main(void){
	MYSQL *mysql = mysql_init(NULL);
	mysql_error(mysql);

	mysql_real_connect(mysql, "localhost", "root", "123456", "day1", 0, NULL, 0);
	mysql_error(mysql);

	mysql_query(mysql, "select * from student where math < 60 and chinese < 60 and english < 60");
	mysql_error(mysql);
	MYSQL_RES *res = mysql_store_result(mysql);
	int fields = mysql_num_fields(res);

	MYSQL_ROW row;
	while(row = mysql_fetch_row(res)){
		for(int i=0; i < fields; i++){
			printf("%s\t", row[i]);
		}
		printf("\n");
	}

	mysql_free_result(res);

	mysql_query(mysql, "select province,count(id) as total,GROUP_CONCAT(name) as list from city where country = '中国' group by province");
	mysql_error(mysql);
	res = mysql_store_result(mysql);
	fields = mysql_num_fields(res);

	while((row = mysql_fetch_row(res))){
		for(int i=0; i < fields; i++){
			printf("%s\t", row[i]);
		}
		printf("\n");
	}
	mysql_free_result(res);
	mysql_close(mysql);
	return  0;
}
