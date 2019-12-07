import React from 'react'

import './index.less'

const NormalCard = ({type, data, bool}) => {
 
  switch (type) {
    case "heartRate":
      if (data) {
        data += " BPM"
      }
      break
    default:
      break
  }
  return (
    <h3>
      {(data ==='') ? "Nenhum dado encontrado" : (!bool) ? data : (data) ? "Algo de errado aconteceu!" : "Está tudo bem"}
   </h3>
  )
}

export default NormalCard
