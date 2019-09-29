
create database car_information
drop table car_information;

CREATE TABLE `car_information` (
  `id` bigint(11) unsigned NOT NULL AUTO_INCREMENT COMMENT '自增主键',
  `license_plate_number` varchar(128) DEFAULT NULL COMMENT '车牌号',
  `user_name` varchar(128) DEFAULT NULL COMMENT '车主姓名',
  `user_gender` int(10) DEFAULT 2 COMMENT '车主性别，0为男1为女,2为初始状态，即还未设定性别',
  `car_type` varchar(128) DEFAULT NULL COMMENT '车辆种类',
  `swept_volumn` float(5,2) DEFAULT 0.00 COMMENT '车排量',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=48 DEFAULT CHARSET=utf8mb4;

ALTER TABLE car_information AUTO_INCREMENT=0;


# 复制表结构

CREATE TABLE zhihu_information_question_275359100_test LIKE zhihu_information_question_275359100; 
INSERT INTO zhihu_information_question_275359100_test SELECT * FROM zhihu_information_question_275359100;


select voteup_count from  zhihu_information_question_275359100  where id<1000;

select voteup_count from  zhihu_information_question_275359100  where id<100;

select content from  zhihu_information_question_275359100 where voteup_count>100 and author_gender=1 order by voteup_count desc limit 1;

select author_gender from  (select * from  zhihu_information_question_275359100  where voteup_count>100)  where author_gender=1;