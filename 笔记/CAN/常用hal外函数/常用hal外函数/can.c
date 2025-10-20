#include<stdio.h>
// 初始化CAN总线
void CAN_Init(CAN_HandleTypeDef* hcan) {
	HAL_CAN_Start(hcan);
	__HAL_CAN_ENABLE_IT(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
	__HAL_CAN_ENABLE_IT(hcan, CAN_IT_RX_FIFO1_MSG_PENDING);
}
// 发送数据帧
uint8_t CAN_Send_Data(CAN_HandleTypeDef* hcan, uint16_t ID, uint8_t* Data, uint8_t Length) {
	CAN_TxHeaderTypeDef tx_header;
	uint32_t used_mailbox;
	// 检测关键传参
	assert_param(hcan != NULL);

	tx_header.StdId = ID;
	tx_header.ExtId = 0;
	tx_header.IDE = 0;
	tx_header.RTR = 0;
	tx_header.DLC = Length;

	return (HAL_CAN_AddTxMessage(hcan, &tx_header, Data, &used_mailbox));
}
// 滤波器
void CAN_Filter_Mask_Config(CAN_HandleTypeDef* hcan, uint8_t Object_Para, uint32_t ID, uint32_t Mask_ID) {
	CAN_FilterTypeDef can_filter_init_structure;
	assert_param(hcan != NULL);
	if ((Object_Para & 0x02)) {
		// 数据帧
		// 掩码后ID的高16位bit
		can_filter_init_structure.FilterIdHigh = ID << 3 << 16;
		//掩码后ID的低16位bit
		can_filter_init_structure.FilterIdLow = ID << 3 | ((Object_Para & 0x03) << 1);
		// ID掩码值高16bit
		can_filter_init_structure.FilterMaskIdHigh = Mask_ID << 3 << 16;
		// ID掩码值低16bit
		can_filter_init_structure.FilterMaskIdLow = Mask_ID << 3 | ((Object_Para & 0x03) << 1);
	}
	else {
		// 遥控帧
		//掩码后ID的高16位bit
		can_filter_init_structure.FilterIdHigh = ID << 5;
		//掩码后ID的低16位bit
		can_filter_init_structure.FilterIdLow = ((Object_Para & 0x03) << 1);
		// ID掩码值高16bit
		can_filter_init_structure.FilterMaskIdHigh = Mask_ID << 5;
		// ID掩码值低16bit
		can_filter_init_structure.FilterMaskIdLow = ((Object_Para & 0x03) << 1);
	}
	// 滤波器序号,0-27,共28个滤波器,can1是0~13,can2是14~27,c8t6仅有14个滤波器
	can_filter_init_structure.FilterBank = (Object_Para >> 3) & 0x0F;
	// 滤波器绑定FIFOx,只能绑定一个
	can_filter_init_structure.FilterFIFOAssignment = (Object_Para >> 2) & 0x01;
	// 使能滤波器
	can_filter_init_structure.FilterActivation = ENABLE;
	// 滤波器模式,设置ID掩码模式
	can_filter_init_structure.FilterMode = CAN_FILTERMODE_IDMASK;
	// 32位滤波
	can_filter_init_structure.FilterScale = CAN_FILTERSCALE_32BIT;
	// 从机模式选择开始单元
	can_filter_init_structure.SlaveStartFilterBank = 14;
	HAL_CAN_ConfigFilter(hcan, &can_filter_init_structure);
}