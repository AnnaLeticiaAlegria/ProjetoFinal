import React from 'react'

import './index.less'

const AlertCard = ({sendAlert}) => {

  return (
    <div className="alert_button" onClick={() => sendAlert()}>
      Mandar um alerta!
   </div>
  )
}

export default AlertCard
